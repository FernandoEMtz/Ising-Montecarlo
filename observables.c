//
// Created by Fernando on 15/09/2026.
//

#include "observables.h"

#include "parametros.h"

int suma_de_vecinos(const almc_spin_seed_t *ejecucion, int i) {
    int j = 0, k = 0, up = 0, dw = 0, lf = 0, rg = 0;

    j = (i-1)/n_x+1; //Obtencion de la fila
    k = (i-1)%n_x+1; //Obtencion de la columna

    up = ( j == 1 ) ? ejecucion->spin[i + n_x*(n_y-1)] : ejecucion->spin[i - n_x];
    dw = (j == n_y) ? ejecucion->spin[i - n_x*(n_y-1)] : ejecucion->spin[i + n_x];
    lf = ( k == 1 ) ? ejecucion->spin[i + ( n_x - 1) ] : ejecucion->spin[i - 1];
    rg = (k == n_x) ? ejecucion->spin[i - ( n_x - 1) ] : ejecucion->spin[i + 1];

    return (up + dw + lf + rg);
}

/*Se obtiene la magnetizacion por cada spin, es un calco extensivo, para hacer la estadistica es importante dividir
* entre el número total de spines debido a que estamos hablando de la magnetizacion y la susceptibilidad
* magnetica por spin
*/
double magnetizacion(const almc_spin_seed_t *ejecucion) {
    int i = 0;
    double mom_magn = 0;
    for(i = 1; i <= n_sp; i++) {
        mom_magn += ejecucion->spin[i];
    }
    return mom_magn/(double)n_sp;
}

/* Caculamos la energía física total del sistema mediante el hamiltoniano recordemos que se describe mediante
 *
 * H = -j * Sum_{<i, j>} s_i s_j - h_campo * Sum_i s_i
 */
double hamiltoniano(const almc_spin_seed_t *ejecucion) {
    int i = 0;
    double inter_vec = 0.0;

    //Suma de la interacción del spin con las cargas de sus vecinos
    for(i = 1; i <= n_sp; i++) inter_vec += ejecucion->spin[i] * suma_de_vecinos(ejecucion, i);

    //Se usa 0.5 para no contar dos veces cada enlace
    double hamiltoniano = -J * 0.5 * inter_vec - h_campo * (double)n_sp * magnetizacion(ejecucion);

    return hamiltoniano;
}