#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "boltzmann.h"
#include "parametros.h"
#include "gestor_de_datos_de_salida.h"
#include "gestor_de_ejecuciones.h"

/* Version 4 del codigo para el Modelo de Ising para una red simple bidimensional
 *
 * Notas:
 *      //TODO actualizar notas con los cambios que hemos hecho
 *
 * -> Codigo en modulos, parametros a editar para el funcionamiento:
 *          Parametros_c --------> Constantes termodinamicas y constantes de control para Montecarlo y la estadística.
 *          Parametros_h --------> Tamaño de la red y número de vecinos.
 *
 * -> Montecarlo usa el metodo de vecinos para obtener la aportación de energía por medio de un spin aleatorio
 *    el metodo de vecinos está pensado para una red simple bidimensional, sin embargo, se puede hacer un cambio
 *    en esta parte del codigo para implementar redes de diferentes estructuras, como por ejemplo una estructura
 *    honey comb.
 *
 * -> La red de vecinos es una red de movimiento browniano en cadenas de Markov.
 *
 * -> Montecarlo usa el metodo de Metropolis para calcular la energía del sistema y su posterior cambio.
 *
 * -> Los numeros aleatorios se obtienen mediante la aplicación del metodo pcg32 en su modelo pcg_basic, esto para
 *    evitar la remota, pero existente posibilidad de duplicar un numero aleatorio al obtener al mismo tiempo el
 *    valor del reloj.
 *
 * -> La aceptación en el metodo de Metropolis utiliza una lista de factores de Boltzmann para mejorar la eficiencia.
 *
 * -> La lista de factores de Boltzmann está pensada para funcionar con redes de diferentes formas, por lo tanto,
 *    diferente cantidad de vecinos, para esta red se tiene el parametro con valor 4, este parametro se actualiza
 *    junto con el posterior cambio en los vecinos, almacena la cantidad necesaria y se calcula en cada iteración.
 *
 * -> El cálculo del Hamiltoniano considera la aportación del campo magnetico con la variable h_campo, actualmente
 *    la variable h_campo se encuentra en 0.0
 *
 * -> El cálculo del Hamiltoniano también considera el cambio en la constante de acoplamiento físico del material
 *    magnético está representada por la variable J, actualmente se encuentra en 1.0 siendo sumamente magnético.
 *
 * -> La aceptación del movimiento está condicionada a la interacción de los vecinos y del campo magnético, de forma que
 *    la aceptación del cambio se determina con un spin y evita calcular nuevamente el sistema para compararlo con el
 *    anterior logrando reducir enormemente la demanda de poder computacional.
 *
 * -> Se comunica con gnuplot para generar un mapa bidimensional de la configuracion final de los spines, pensado
 *    para su aplicacion en Machine Learning, solo se genera la representacion de los spines en blanco y negro
 *
 * -> Se tiene un módulo encargado de la escritura de todos los datos estadisticos a fin de diferenciarlos de los
 *    modulos para realizar los calculos físicos y estadisticos del ciclo de temperaturas y de Montecarlo.
 *
 * -> Dentro del módulo de escritura de datos también se generan imagenes con ayuda de Gnuplot, se tiene una bandera
 *    que permite elegir si se quiere que se generen las imagenes o no.
 *
 * -> Los datos recopilados se organizan en 4 archivos diferentes:
 *          Datos generales -----> Recopila el reporte de cada ejecución realizada, permite encontrar anomalias
 *          Estadística general -> Recopila cada paso Montecarlo, la energía promedio, la magnetización promedio y la
 *                                 aceptación del movimiento para cada cambio de estado en el sistema.
 *          Resultados generales-> Recopila la temperatura adimensional, energia promedio, susceptibilidad termica,
 *                                 magnetización promedio y susceptibilidad magnetica.
 *          Mapa spin -----------> Genera la configuración final de la red de spines en un arreglo bidimensional
 *          Imagenes ------------> Se usa gnuplot para generar los archivos de imagen en conjunto con los arreglos
 *                                 ya calculados, se ejecuta al terminar toda la ejecucion del archivo.
 *
 * -> Esta versión se encarga de generar varias simulaciones organizando los datos recopilados e indexados, esto para
 *    permitir realizar promedios estadisticos y mejorar las aproximaciones obtenidas, además de facilitar la
 *    posterior consulta si es que existe alguna anomalía en la simulación, por ejemplo un estancamiento en el cambio
 *    de estados que genere un error en la simulación.
 *
 * -> PENDIENTE PARA MEJORAR EL RENDIMIENTO: Realizar un cambio dinamico para iniciar el barrido en los datos
 *    estadisticos, ya que entre más se acera al punto crítico más pasos Montecarlo necesita para estabilizar el sistema
 *    lo que genera estados atascados; en los extremos genera desperdicio de poder de cálculo.
 *
 */

int main(void) {
    int i_ejec = 0;
    int num_ejec = fin_sim - ini_sim +1;
    int tot_hilos_disp = omp_get_max_threads();
    int tot_hilos_usar = ( num_ejec <= hilos) ? num_ejec : hilos;

    omp_set_num_threads(tot_hilos_usar);

    //Generacion de semilla con base en el reloj
    uint64_t semilla = (uint64_t) time(NULL);
    //Pre-generación de las carpetas para que la escritura paralela no tenga problemas de ejecucion
    genera_carpetas();
    //Pre-generacion tablas Boltzmann para evitar recalculos innecesarios

    system("cls");
    printf("\n\n  ============================= Modelo de Ising paralelizado =========================== \n");
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\tTotal de ejecuciones a realizar:\t%d\t\t\t||\n", num_ejec);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\tNumero de hilos a utilizar durante la ejecucion %d de %d (capado a %d de %d)\t||\n",tot_hilos_usar, hilos, hilos, tot_hilos_disp);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("  ======================================================================================\n");
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\t\tCalculando tablas de Boltzmann\t\t\t\t||\n");
    //Pre-generacion tablas Boltzmann para evitar recalculos innecesarios
    tabla_boltzmann_t tablas_temp[n_temp+1];
    pre_calcula_tabla_boltzmann(tablas_temp);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\t\tTablas de Boltzmann calculadas\t\t\t\t||\n");
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("||\t\t\tTotal de tablas calculadas por ejecucion: \t%d\t\t||\n", tablas_temp[0].tam);
    printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
    printf("  ======================================================================================\n");
    //Paralelizacion
    #pragma omp parallel for
    for (i_ejec = ini_sim; i_ejec <= fin_sim; ++i_ejec) {
        recorrido_de_temperaturas(tablas_temp, semilla, i_ejec);
    }
    libera_memoria_tabla_temperatura_boltzmann(tablas_temp);
    printf("  ======================================================================================\n");
    if (plot_mapa) {
        printf("||\t\t\t\tInicio de generacion de imagenes\t\t\t||\n");
        printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
        ejecuta_generador_imagenes_spines();
        printf("||\t\t\t\t\t\t\t\t\t\t\t||\n");
        printf("||\t\t\t\tImagenes generadas con exito\t\t\t\t||\n");
        printf("  ======================================================================================\n");
    }
    return 0;
}

// gcc -O2 -lm -fopenmp main.c boltzmann.c gestor_de_datos_de_salida.c gestor_de_ejecuciones.c gestor_inicializacion_y_randomizador.c montecarlo.c observables.c parametros.c pcg_basic.c -o ejecuta_V4