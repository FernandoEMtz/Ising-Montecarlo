//
// Created by Fernando on 16/09/2026.
//

#ifndef MODELO_ISING_C_V4_MONTECARLO_H
#define MODELO_ISING_C_V4_MONTECARLO_H

#include <stdio.h>

#include "boltzmann.h"
#include "gestor_inicializacion_y_randomizador.h"

//Este struct almacenará los resultados termodinamicos promedio que guardaremos en las diversas salidas
typedef struct {
    double ener_prom;   //energía promedio
    double magn_prom;   //magnetizacion promedio
    double susc_ter;    //susceptibilidad termica -> c_v
    double susc_magn;   //susceptibilidad magnetica -> chi
}resultados_montecarlo_t;

resultados_montecarlo_t montecarlo(almc_spin_seed_t *ejecucion, const tabla_boltzmann_t *tabla, FILE *archivo_estadistica, double ener_ini, double temp_star);

#endif //MODELO_ISING_C_V4_MONTECARLO_H
