//
// Created by Fernando on 15/09/2026.
//

#ifndef MODELO_ISING_C_V4_OBSERVABLES_H
#define MODELO_ISING_C_V4_OBSERVABLES_H

#include "parametros.h"

int suma_de_vecinos (const almc_spin_seed_t *ejecucion, int i);
double magnetizacion(const almc_spin_seed_t *ejecucion);
double hamiltoniano (const almc_spin_seed_t *ejecucion);

#endif //MODELO_ISING_C_V4_OBSERVABLES_H
