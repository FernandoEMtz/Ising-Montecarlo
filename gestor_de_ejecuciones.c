//
// Created by Fernando on 14/09/2026.
//

#include <stdio.h>

#include "gestor_de_ejecuciones.h"

#include "parametros.h"
#include "gestor_de_datos_de_salida.h"
#include "observables.h"
#include "boltzmann.h"
#include "montecarlo.h"
#include "gestor_inicializacion_y_randomizador.h"

void recorrido_de_temperaturas(const tabla_boltzmann_t *tablas_temp, uint64_t semilla, int i_ejec) {
    int i_temp = 0;
    double temp_star = 0.0, ener_ini = 0.0;

    almc_spin_seed_t ejecucion;
    resultados_montecarlo_t resultado_mc;

    FILE *archivo_reporte = NULL;
    FILE *archivo_spines = NULL;
    FILE *archivo_estadistica = NULL;
    FILE *archivo_resultados = NULL;

    archivo_resultados = crea_archivo_resultados_finales(i_ejec);

    //Inicializacion de pcg32, pasamos la semilla generada por el reloj y para cada ejecucion
    inicia_pcg32_paralelo(&ejecucion.generador_rng, semilla, i_ejec);

    //recorrido de cada temperatura y escritura de los datos
    for (i_temp = 1; i_temp <= n_temp; i_temp++) {
        temp_star = calcula_temp_star(i_temp);
        //Inicializamos el arreglo de spines inicial para cada sistema
        inicializacion_arreglo_spines_ejecucion(&ejecucion);
        //Obtenemos el hamiltoniano para este primer sistema
        ener_ini = hamiltoniano(&ejecucion);

        archivo_estadistica = crea_estadistica_por_paso_montecarlo(i_ejec, i_temp);
        resultado_mc = montecarlo(&ejecucion, &tablas_temp[i_temp], archivo_estadistica, ener_ini, temp_star);
        fclose(archivo_estadistica);

        archivo_reporte = crea_reporte_de_simulacion(i_ejec, i_temp);
        escribe_reporte_de_simulacion(archivo_reporte, i_ejec, i_temp, temp_star, resultado_mc.ener_prom, resultado_mc.susc_ter, resultado_mc.magn_prom, resultado_mc.susc_magn);
        escribe_resultados_finales(archivo_resultados, temp_star, resultado_mc.ener_prom, resultado_mc.susc_ter, resultado_mc.magn_prom, resultado_mc.susc_magn);
        archivo_spines = crea_configuracion_final_de_spines(i_ejec, i_temp);
        escribe_configuracion_final_de_spines(archivo_spines, &ejecucion);

        imprime_avance_ejecucion(i_ejec, i_temp);

    }

    fclose(archivo_resultados);

}
double calcula_temp_star(int i_temp) {
    return temp_i + i_temp * (temp_f - temp_i)/(double)n_temp;
}