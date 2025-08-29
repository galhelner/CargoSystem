cmake_minimum_required(VERSION 3.26)
project(HW2)

set(CMAKE_CXX_STANDARD 23)

add_executable(HW2 main.cpp
        Port.h
        ContainersGraph.h
        ContainersGraph.cpp
        TimesGraph.h
        TimesGraph.cpp
        Date.h
        Date.cpp
        PortNotExistsException.h
        SailContainersData.h
        FileErrorException.h
        SystemUtils.h
        SystemUtils.cpp
        InvalidInputException.h
        SailTimesData.h
        Commands.h
        DateFormatException.h
        Commands.cpp
)
