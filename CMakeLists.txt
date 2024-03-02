cmake_minimum_required(VERSION 3.27)
project(Water_Supply_Analysis_System)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Water_Supply_Analysis_System main.cpp
        App.cpp
        Graph.cpp
        Data.cpp
        WaterReservoir.cpp
        PumpingStation.cpp
        DeliverySite.cpp
        Pipe.cpp
        States/MainMenuState.cpp
        States/State.cpp)
