cmake_minimum_required(VERSION 3.17)
project(unifi-fbw777 C)

set(CMAKE_C_STANDARD 99)

add_executable(aeroplanetty src/main.c src/failureGenerator.c src/transducers.c src/wes.c src/pfc.c src/headers/pfc.h src/headers/trasducers.h src/headers/wes.h)