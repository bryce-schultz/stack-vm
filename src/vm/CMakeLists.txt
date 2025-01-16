# VM project

add_executable(vm
    src/main.cpp
    src/SimpleVirtualMachine.cpp
    parser/src/SVASMParser.cpp
    exception/src/SVMException.cpp
)

target_include_directories(vm PRIVATE
    include
    parser
    exception/include
)