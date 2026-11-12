//
// Created by Fernando on 15/09/2026.
//

#ifndef MODELO_ISING_C_V4_GESTOR_INICIALIZACION_Y_PARALELIZACION_H
#define MODELO_ISING_C_V4_GESTOR_INICIALIZACION_Y_PARALELIZACION_H

#include "pcg_basic.h"
#include "parametros.h"

void inicia_pcg32_paralelo(pcg32_random_t *random_local, uint64_t semilla, int i_ejec);
double randomizador_paralelo(pcg32_random_t *random_local);
void inicializacion_arreglo_spines_ejecucion(almc_spin_seed_t *ejecucion);

#endif //MODELO_ISING_C_V4_GESTOR_INICIALIZACION_Y_PARALELIZACION_H
