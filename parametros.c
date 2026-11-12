//
// Created by Fernando on 14/09/2026.
//

#include <stdbool.h>

#include "parametros.h"

/* ---------------------------------- CONSTANTES TERMODINÁMICAS ---------------------------------- */
const double temp_i = 1.0;   //Temperatura reducida inicial
const double temp_f = 3.0;   //Temperatura reducida final
const int n_temp    = 200;   //Número total de temperaturas para realizar el barrido
const double h_campo= 0.0;   //Campo magnético externo reducido
const double J      = 1.0;   //Constante de acoplamiento fisica, es adimensional y determina el
//comportamiento de un material, es parte del Hamiltoniano
/* ---- ************************************************************************************* ---- */

/* -- CONSTANTES DE CONTROL PARA MONTECARLO, ESTADÍSTICA DEL SISTEMA E INICIO DE LA SIMULACIÓN --- */
const int n_mc      = 12000; //Número de pasos para Montecarlo por cada temperatura
//const int n_out     = 1000;  //Cada cuanto se imprime el avance en pantalla
const int n_start   = 6000;  //A partir de que paso de Montecarlo se realizará la estadística
const int ini_sim   = 51;    //Inicio de las simulaciones a ejecutar
const int fin_sim   = 100;    //Limite superior a las simulaciones por ejecucion, actualizar cada que
//se quiera acumular simulaciones
const bool plot_mapa = true;  //Bandera de control para generar las imagenes de los mapas de spines
const int hilos     = 10;   //Número de hilos del procesador a ocupar
/* ---- ************************************************************************************* ---- */
