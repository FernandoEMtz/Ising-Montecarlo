//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_C_V4_ALMACENAMIENTO_DE_DATOS_H
#define MODELO_ISING_C_V4_ALMACENAMIENTO_DE_DATOS_H

#include "parametros.h"

/* ------------------------------------ GENERADOR DE CARPETAS ------------------------------------ */
void genera_carpetas();
void mensaje_error(char *nombre_archivo);
/* ---- ************************************************************************************* ---- */

/* ------------------------------- ESTADÍSTICA POR PASO MONTECARLO ------------------------------- */
FILE *crea_estadistica_por_paso_montecarlo(int i_ejec, int i_temp);
void escribe_estadistica_por_paso_montecarlo(FILE *archivo_estadistica, int i_mc, double ener_i,
     double magn_i, double accpt);
/* ---- ************************************************************************************* ---- */

/* ------------------------- RESULTADOS FINALES OBTENIDOS POR SIMULACION ------------------------- */
FILE *crea_archivo_resultados_finales(int i_ejec);
void escribe_resultados_finales(FILE *archivo_resultados, double temp_star, double ener_prom,
     double susc_ter, double magn_prom, double susc_magn);
/* ---- ************************************************************************************* ---- */

/* ---------------------------- REPORTE POR CADA TEMPERATURA SIMULADA ---------------------------- */
FILE *crea_reporte_de_simulacion(int i_ejec, int i_temp);
void escribe_reporte_de_simulacion(FILE *archivo_reporte, int i_ejec, int i_temp, double temp_star,
     double ener_prom, double susc_ter, double magn_prom, double susc_magn);
/* ---- ************************************************************************************* ---- */

/* -------------------------------- CONFIGURACION FINAL DE SPINES -------------------------------- */
FILE *crea_configuracion_final_de_spines(int i_ejec, int i_temp);
void escribe_configuracion_final_de_spines(FILE *archivo_spines, const almc_spin_seed_t *ejecucion);

/* --------------- CONFIGURACION GENERADOR DE IMAGENES DE MICROESTADOS VIA GNUPLOT --------------- */
void inicia_generador_imagenes_spines();
void genera_imagenes_spines(int i_ejec, int i_temp);
void finaliza_imagenes_spines();
void ejecuta_generador_imagenes_spines();
/* ---- ************************************************************************************* ---- */

/* ---- *********** DATOS DE SALIDA EN PANTALLA  PARA NOTAR EL AVANCE EN PARALELO *********** ---- */
void cadena_porcentaje(char caracter, int num);
void imprime_avance_ejecucion(int i_ejec, int i_temp);
/* ---- ************************************************************************************* ---- */
#endif //MODELO_ISING_C_V4_ALMACENAMIENTO_DE_DATOS_H
