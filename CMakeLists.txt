cmake_minimum_required(VERSION 3.16)

project(
    EhBASICEmu
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin
)

add_executable(EhBASICEmu
    src/Emulator.cpp
    src/main.cpp
    src/CPU.cpp
    src/CPU_Flags.cpp
    src/CPU_InstructionSet.cpp
    src/CPU_IO.cpp
    src/Bus.cpp
    src/SerialPort.cpp
)

target_include_directories(
    EhBASICEmu
    PRIVATE
    include
)