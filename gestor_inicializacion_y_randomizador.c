//
// Created by Fernando on 15/09/2026.
//

#include "gestor_inicializacion_y_randomizador.h"

#include "pcg_basic.h"
#include "parametros.h"

/* El modelo de PCG32 permite implementarse para metodos paralelos sin que se cruce la generacion de numeros aleatorios
 * durante la ejecucion de los diferentes hilos, esto se hace generando una instancia para cada hilo y podemos usar
 * una semilla base para cada hilo, esto con el fin de evitar generaciones del mismo número aleatorio en diferentes
 * ejecuciones al compartir la misma lectura del reloj.
 */

//Se tiene que inicializar el generador para que cada hilo sea diferente a los demás
void inicia_pcg32_paralelo(pcg32_random_t *random_local, uint64_t semilla, int i_ejec) {

    //pasamos el valor al random local, usamos semilla y a i_ejec para que cada hilo tenga su propio generador
    pcg32_srandom_r(random_local, semilla, (uint64_t) i_ejec);
}

double randomizador_paralelo(pcg32_random_t *random_local) {
    //Igualmente, generamos el número aleatorio [0:1] con base al random local para cada hilo
    return (double)pcg32_random_r(random_local)/((double)UINT32_MAX+1);
}

/* Tendremos un arreglo de spines unico para cada ejecucion, anteriormente era global, ahora lo tenemos dentro de un
* struct, ya que cada configuración será unica y se modificará de manera única para cada ejecucion
*/
void inicializacion_arreglo_spines_ejecucion(almc_spin_seed_t *ejecucion) {
    int i = 0;
    double randomcito = 0.0;
    for (i = 1; i <= n_sp; i++) {
        //randomizador paralelo apunta a la semilla generada para cada random local, almacenado en generador_rng
        randomcito = randomizador_paralelo(&ejecucion->generador_rng);
        ejecucion->spin[i] = (randomcito < 0.5) ? -1: 1;
    }
}