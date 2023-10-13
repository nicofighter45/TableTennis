cmake_minimum_required(VERSION 3.26)
project(TableTennis)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(processing)
include_directories(simulations)
include_directories(tools)

add_executable(TableTennis
        data/en.txt
        "Interpolation de point/abstract.py"
        "Interpolation de point/TEST d'interpolation.py"
        "Interpolation de point/Test Regression lin�aire"
        "Interpolation de point/Test Regression lin�aire.py"
        old/simulation.c
        processing/analyse.cpp
        processing/analyse.hpp
        processing/area.cpp
        processing/area.hpp
        processing/configuration.cpp
        processing/configuration.hpp
        processing/prompt.cpp
        processing/prompt.hpp
        processing/tracking.cpp
        processing/tracking.hpp
        simulations/friction.h
        simulations/gravity.h
        simulations/magnus.h
        tools/constants.hpp
        tools/export.hpp
        tools/python_link.hpp
        tools/vector.cpp
        tools/vector.hpp
        tracking/tracking.cpp
        .gitignore
        data_tipe.txt
        LICENSE
        loader.cpp
        loader.hpp
        main.cpp
        README.md
        table_tennis.ipynb)
