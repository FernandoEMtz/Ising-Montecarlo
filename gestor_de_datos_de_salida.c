//
// Created by Fernando on 14/09/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "gestor_de_datos_de_salida.h"

#include "parametros.h"

static FILE *gnuplot_spines = NULL;

void genera_carpetas() {
    char nombre_carpeta[FILENAME_MAX];
    int i_temp = 0, j_ejec = 0;

    //Comprobamos si es que aún no existen las carpetas en caso de que no, las generamos, capamos para generar
    //tanto en Linux como en windows
    #ifdef _WIN32
        (void)system("if not exist Reportes_de_simulaciones mkdir Reportes_de_simulaciones");
        (void)system("if not exist Configuracion_final_de_spines mkdir Configuracion_final_de_spines");
        (void)system("if not exist Estadistica_por_paso_Montecarlo mkdir Estadistica_por_paso_Montecarlo");
        (void)system("if not exist Resultados_finales mkdir Resultados_finales");
    #else
        (void)system("mkdir Reportes_de_simulaciones");
        (void)system("mkdir Configuracion_final_de_spines");
        (void)system("mkdir Estadistica_por_paso_Montecarlo");
        (void)system("mkdir Resultados_finales");
    #endif

    //Creacion de las subcarpetas
    //Configuracion final de spines depende de las iteraciones de cada temperatura
    //Reporte de simulaciones y Estadistica por paso de montecarlo dependen de las ejecuciones

    //Configuracion final de spines
    for (i_temp = 1; i_temp <= n_temp; i_temp++) {
    #ifdef _WIN32
        sprintf(nombre_carpeta,"if not exist Configuracion_final_de_spines\\Mapa_de_dominios_magneticos_%d mkdir Configuracion_final_de_spines\\Mapa_de_dominios_magneticos_%d", i_temp, i_temp);
    #else
        sprintf(nombre_carpeta, "mkdir -p Configuracion_final_de_spines/Mapa_de_dominios_magneticos_%d", i_temp);
    #endif
        (void)system(nombre_carpeta);
    }
    //Reporte de simulaciones y estadistica por paso de montecarlo
    for (j_ejec = ini_sim; j_ejec <= fin_sim; j_ejec++) {
        //Vamos a crear una carpeta para cada ejecucion
        #ifdef _WIN32
            sprintf(nombre_carpeta,"if not exist Estadistica_por_paso_Montecarlo\\Estadistica_de_la_ejecucion_%d mkdir Estadistica_por_paso_Montecarlo\\Estadistica_de_la_ejecucion_%d", j_ejec, j_ejec);
            (void)system(nombre_carpeta);
            sprintf(nombre_carpeta,"if not exist Reportes_de_simulaciones\\Reporte_de_la_ejecucion_%d mkdir Reportes_de_simulaciones\\Reporte_de_la_ejecucion_%d", j_ejec, j_ejec);
            (void)system(nombre_carpeta);
        #else
            sprintf(nombre_carpeta, "mkdir -p Estadistica_por_paso_Montecarlo/Estadistica_de_la_ejecucion_%d", j_ejec);
            (void)system(nombre_carpeta);
            sprintf(nombre_carpeta, "mkdir -p Reportes_de_simulaciones/Reporte_de_la_ejecucion_%d", j_ejec);
            (void)system(nombre_carpeta);
        #endif
    }
}

FILE *crea_reporte_de_simulacion(int i_ejec, int i_temp) {
    char nombre_archivo[FILENAME_MAX];
    FILE *archivo_reporte = NULL;

    sprintf(nombre_archivo, "Reportes_de_simulaciones/Reporte_de_la_ejecucion_%d/reporte_tecnico_%d_de_%d.dat", i_ejec, i_temp, n_temp);
    archivo_reporte = fopen(nombre_archivo, "w");

    if (archivo_reporte == NULL) {
        mensaje_error(nombre_archivo);
    }
    return archivo_reporte;
}

FILE *crea_configuracion_final_de_spines(int i_ejec, int i_temp) {
    char nombre_archivo[FILENAME_MAX];
    FILE *archivo_spines = NULL;
    sprintf(nombre_archivo,"Configuracion_final_de_spines/Mapa_de_dominios_magneticos_%d/microestado_%d_ejecucion_%d.dat", i_temp, i_temp, i_ejec);
    archivo_spines = fopen(nombre_archivo, "w");

    if (archivo_spines == NULL) {
        mensaje_error(nombre_archivo);
    }
    return archivo_spines;
}

FILE *crea_estadistica_por_paso_montecarlo(int i_ejec, int i_temp) {
    char nombre_archivo[FILENAME_MAX];
    FILE *archivo_estadistica = NULL;

    sprintf(nombre_archivo, "Estadistica_por_paso_Montecarlo/Estadistica_de_la_ejecucion_%d/montecarlo_i_temp_%d_de_%d.dat", i_ejec, i_temp, n_temp);
    archivo_estadistica = fopen(nombre_archivo, "w");

    if (archivo_estadistica == NULL) {
        mensaje_error(nombre_archivo);
    }
    return archivo_estadistica;
}

FILE *crea_archivo_resultados_finales(int i_ejec) {
    char nombre_archivo[FILENAME_MAX];
    FILE *archivo_resultados = NULL;

    sprintf(nombre_archivo, "Resultados_finales/resultado_ejecucion_%d.dat", i_ejec);
    archivo_resultados = fopen(nombre_archivo, "w");

    if (archivo_resultados == NULL) {
        mensaje_error(nombre_archivo);
    }

    return archivo_resultados;
}

void escribe_reporte_de_simulacion(FILE *archivo_reporte, int i_ejec, int i_temp, double temp_star, double ener_prom, double susc_ter, double magn_prom, double susc_magn) {
    fprintf(archivo_reporte," --- ******************************************************* --- \n");
    fprintf(archivo_reporte,"|\t\t\t\t\t\t\t\t|\n");
    fprintf(archivo_reporte,"|\t\tReporte de Simulacion de Ising\t\t\t|\n");
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte,"*\tEjecucion de la simulacion numero:\t%d\t\t*\n",i_ejec);
    fprintf(archivo_reporte,"*\tBarrido de temperatura :\t\t%d de %d\t*\n", i_temp, n_temp);
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte,"*\tTotal de pasos Montecarlo:\t\t%d\t\t*\n", n_mc);
    fprintf(archivo_reporte,"*\tPasos usados para la estadistica:\t%d\t\t*\n", n_mc - n_start);
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte,"*\tTamano de la red:\t\t\t%d x %d\t\t*\n", n_x, n_y);
    fprintf(archivo_reporte,"*\tNumero de spines:\t\t\t%d\t\t*\n", n_sp);
    fprintf(archivo_reporte,"*\t\t\t\t\t\t\t\t*\n");
    fprintf(archivo_reporte,"*\tTemperatura reducida:\t\t\t%.6f\t*\n", temp_star);
    fprintf(archivo_reporte,"*\tEnergia promedio del sistema:\t\t%.8f\t*\n", ener_prom);
    fprintf(archivo_reporte,"*\tSusceptibilidad termica (Cv):\t\t%.8f\t*\n", susc_ter);
    fprintf(archivo_reporte,"*\tMagnetizacion promedio del sistema:\t%.8f\t*\n",magn_prom);
    fprintf(archivo_reporte,"|\tSusceptibilidad magnetica (Chi):\t%.8f\t|\n",susc_magn);
    fprintf(archivo_reporte,"|\t\t\t\t\t\t\t\t|\n");
    fprintf(archivo_reporte," --- ******************************************************* --- \n");

    fclose(archivo_reporte);
}

void escribe_configuracion_final_de_spines(FILE *archivo_spines, const almc_spin_seed_t *ejecucion) {
    int i = 0;
    for (i = 1; i <= n_sp; i++) {
        fprintf(archivo_spines, "%d ", ejecucion->spin[i] );
        if (i % n_x == 0) fprintf(archivo_spines, "\n");
    }
    fclose(archivo_spines);
}

void escribe_estadistica_por_paso_montecarlo(FILE *archivo_estadistica, int i_mc, double ener_i, double magn_i, double accpt) {
    fprintf(archivo_estadistica,"%d %15.8f %15.8f %15.8f\n", i_mc, ener_i, magn_i, accpt);
}

void escribe_resultados_finales(FILE *archivo_resultados, double temp_star, double ener_prom, double susc_ter, double magn_prom, double susc_magn) {
    fprintf(archivo_resultados, "%.8f %.8f %.8f %.8f %.8f\n", temp_star, ener_prom, susc_ter, magn_prom, susc_magn);
}

void imprime_estadistica_por_paso_montecarlo(int i_mc, double ener_i, double magn_i, double accpt) {
    printf("||\t%d\t||\t%.2f\t||\t%.2f\t||\t%.4f\t\t||\n", i_mc, ener_i, magn_i, accpt);
}

void inicia_generador_imagenes_spines() {
    gnuplot_spines = popen("gnuplot -persist", "w");

    if (gnuplot_spines == NULL) {
        fprintf(stderr, "ERROR en la generacion de imagenes por parte de gnuplot\n");
        exit(EXIT_FAILURE);
    }

    fprintf(gnuplot_spines,"set terminal pngcairo size 512, 512\n");
    fprintf(gnuplot_spines,"set size square\n");
    fprintf(gnuplot_spines,"set autoscale fix\n");
    fprintf(gnuplot_spines,"set tmargin 0\n");
    fprintf(gnuplot_spines,"set bmargin 0\n");
    fprintf(gnuplot_spines,"set lmargin 0\n");
    fprintf(gnuplot_spines,"set rmargin 0\n");
    fprintf(gnuplot_spines,"unset title\n");
    fprintf(gnuplot_spines,"unset colorbox\n");
    fprintf(gnuplot_spines,"unset key\n");
    fprintf(gnuplot_spines,"set xrange[*:*]\n");
    fprintf(gnuplot_spines,"set yrange[*:*] reverse\n");
    fprintf(gnuplot_spines,"set palette define (-1 'black', 1 'white')\n");
    fprintf(gnuplot_spines,"set cbrange [-1:1]\n");
}

void genera_imagenes_spines(int i_ejec, int i_temp) {
    fprintf(gnuplot_spines,"set output \"Configuracion_final_de_spines/Mapa_de_dominios_magneticos_%d/microestado_%d_ejecucion_%d.png\"\n", i_temp, i_temp, i_ejec);
    fprintf(gnuplot_spines,"plot \"Configuracion_final_de_spines/Mapa_de_dominios_magneticos_%d/microestado_%d_ejecucion_%d.dat\" matrix with image\n", i_temp, i_temp, i_ejec);
    fflush(gnuplot_spines);
}

void finaliza_imagenes_spines() {
    if (gnuplot_spines !=NULL) pclose(gnuplot_spines);
}

void mensaje_error(char *nombre_archivo) {
    fprintf(stderr, "Error en la escritura: No se pudo crear %s\n", nombre_archivo);
    exit(EXIT_FAILURE);
}

void ejecuta_generador_imagenes_spines() {
    int i_ejec = 0, i_temp;
    inicia_generador_imagenes_spines();
    for (i_ejec = ini_sim; i_ejec <= fin_sim; i_ejec++) {
        for (i_temp = 1; i_temp <= n_temp; i_temp++) {
            genera_imagenes_spines(i_ejec, i_temp);
        }
    }
    finaliza_imagenes_spines();
}

void cadena_porcentaje(char caracter, int num) {
    if (num <= 0) return;
    putchar(caracter);
    cadena_porcentaje(caracter, num-1);
}

void imprime_avance_ejecucion(int i_ejec, int i_temp) {
    int lim_sup = (i_temp * 10)/n_temp, lim_inf = ((i_temp - 1) * 10)/n_temp, porcentaje;
    if (lim_sup == lim_inf) return;
    porcentaje = lim_sup * 10;

#pragma omp critical
    {
        printf("||\tHilo %d\t\t||\tEjecucion %d\t||\tAvance: %d%%", omp_get_thread_num(), i_ejec, porcentaje);
        printf("\t[");
        cadena_porcentaje('*', lim_sup);
        cadena_porcentaje('-', 10 - lim_sup);
        printf("]\t||\n");
    }
}