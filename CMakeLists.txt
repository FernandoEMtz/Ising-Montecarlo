cmake_minimum_required(VERSION 4.2)
project(Modelo_Ising_C_V4 C)

set(CMAKE_C_STANDARD 11)
find_package(OpenMP REQUIRED)

add_executable(Modelo_Ising_C_V4 main.c
        boltzmann.c
        boltzmann.h
        gestor_de_datos_de_salida.c
        gestor_de_datos_de_salida.h
        gestor_de_ejecuciones.c
        gestor_de_ejecuciones.h
        gestor_inicializacion_y_randomizador.c
        gestor_inicializacion_y_randomizador.h
        montecarlo.c
        montecarlo.h
        observables.c
        observables.h
        parametros.c
        parametros.h
        pcg_basic.c
        pcg_basic.h)

target_link_libraries(Modelo_Ising_C_V4 PRIVATE OpenMP::OpenMP_C)