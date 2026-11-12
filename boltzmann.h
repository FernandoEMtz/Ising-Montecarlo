//
// Created by Fernando on 14/09/2026.
//

#ifndef MODELO_ISING_C_V4_TABLA_BOLTZMANN_H
#define MODELO_ISING_C_V4_TABLA_BOLTZMANN_H

/* Recuerda añadir la descripcion matematica del porqué el arreglo es de n_vec + 1 entradas
 * Los índices se crean con (p+z)/2 donde z es el número de vecinos.
 */

typedef struct {
    double *valores;    //Valores para la tabla boltzmann
    int tam;            //Almacenamiento dinamico del tamaño del arreglo, z+1
}tabla_boltzmann_t;

/* La tabla se construye teniendo en cuenta el número de vecinos para cualquier tipo de red, para el cálculo de exp
 * tenemos en cuenta la constante de acoplamiento y el número de vecinos posibles, hay que hacer que solo se llame
 * una vez por ejecución.
 */
tabla_boltzmann_t calculo_tabla_boltzmann(double temp_star);
double consulta_tabla_boltzmann(const tabla_boltzmann_t *tabla, int prod_spin_vec, int spin_actual);
void libera_memoria_tabla_boltzmann(tabla_boltzmann_t *tabla);
void pre_calcula_tabla_boltzmann(tabla_boltzmann_t *tablas_temp);
void libera_memoria_tabla_temperatura_boltzmann(tabla_boltzmann_t *tablas_temp);
#endif //MODELO_ISING_C_V4_TABLA_BOLTZMANN_H
