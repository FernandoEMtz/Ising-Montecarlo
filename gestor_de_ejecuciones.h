//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_C_V4_GESTOR_DE_EJECUCIONES_H
#define MODELO_ISING_C_V4_GESTOR_DE_EJECUCIONES_H

#include "boltzmann.h"
#include "pcg_basic.h"

void recorrido_de_temperaturas(const tabla_boltzmann_t *tablas_temp, uint64_t semilla, int i_ejec);
double calcula_temp_star(int i_temp);

#endif //MODELO_ISING_C_V4_GESTOR_DE_EJECUCIONES_H
