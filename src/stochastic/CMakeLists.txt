# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(stochastic VERSION 0.1.0 LANGUAGES CXX)

add_library(stochastic SHARED
        Candidate.h
        PopulationSelector.h
        Populator.h
        RNG.h
        RNG.cpp
        )