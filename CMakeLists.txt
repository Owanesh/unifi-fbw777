cmake_minimum_required(VERSION 3.17)
project(unifi-fbw777 C)

set(CMAKE_C_STANDARD 99)

add_executable(aeroplanetty src/main.c src/failureGenerator.c src/transducers.c src/wes.c src/pfc.c src/headers/pfc.h src/headers/transducers.h src/headers/wes.h src/util/utilities.c src/util/headers/utilities.h src/util/headers/constant.h src/util/signals.c src/util/headers/signals.h src/headers/failureGenerator.h src/headers/pfcDisconnectSwitch.h src/pfcDisconnectSwitch.c src/headers/standard.h)