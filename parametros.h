//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_C_V4_PARAMETROS_H
#define MODELO_ISING_C_V4_PARAMETROS_H

#include <stdbool.h>

#include "pcg_basic.h"
// Variables globales, solo se modifican los valores de este archivo y de "Parametros_c"

/* ----------------------------------- DIMENSIONES DEL SISTEMA ----------------------------------- */
#define n_x   40             //Numero de columnas en la red
#define n_y   40             //Numero de filas en la red
#define n_sp  (n_x*n_y)      //Numero total de spines en la red bidimensional
#define n_vec 4              //Número de vecinos, para la red 2D de 4 vecinos, por si se trabaja en
                             //redes más complejas en un futuro
/* ---- ************************************************************************************* ---- */

/* ------------ ESTRUCTURA DE ALMACENAMIENTO PARA CADA EJECUCION DEL ARREGLO DE SPINES ----------- */
typedef struct {
    int spin[n_sp+1];               //Nunca se usa Spin[0], por ello el tamaño maximo es n_sp+1
    pcg32_random_t generador_rng;   //Almacenamiento del número aleatorio
}almc_spin_seed_t;
/* ---- ************************************************************************************* ---- */

/* ---------------------------------- CONSTANTES TERMODINÁMICAS ---------------------------------- */
extern const double temp_i;  //Temperatura reducida inicial
extern const double temp_f;  //Temperatura reducida final
extern const int    n_temp;  //Número total de temperaturas para realizar el barrido
extern const double h_campo; //Campo magnético externo reducido
extern const double J;       //Constante de acoplamiento fisica, es adimensional y determina el
                             //comportamiento de un material, es parte del Hamiltoniano
/* ---- ************************************************************************************* ---- */

/* -- CONSTANTES DE CONTROL PARA MONTECARLO, ESTADÍSTICA DEL SISTEMA E INICIO DE LA SIMULACIÓN --- */
extern const int n_mc;       //Número de pasos para Montecarlo por cada temperatura
//extern const int n_out;      //Cada cuanto se imprime el avance en pantalla
extern const int n_start;    //A partir de que paso de Montecarlo se realizará la estadística
extern const int ini_sim;    //Inicio de las simulaciones a ejecutar
extern const int fin_sim;    //Limite superior a las simulaciones por ejecucion, actualizar cada que
                             //se quiera acumular simulaciones
extern const bool plot_mapa;  //Bandera de control para generar las imagenes de los mapas de spines
extern const int hilos;
/* ---- ************************************************************************************* ---- */

#endif //MODELO_ISING_C_V4_PARAMETROS_H
