cmake_minimum_required(VERSION 3.0)

set(EXTERNAL_INCLUDE_DIRECTORY "./external/")

project(embcli)

add_executable(embdmn src/main.cpp
    src/channel/udpchannel.cpp
    src/protocol/fbprotocol.cpp
    src/dispatcher/dispatcher.cpp)

target_include_directories(embdmn PRIVATE ${EXTERNAL_INCLUDE_DIRECTORY})

install(TARGETS embdmn RUNTIME DESTINATION bin)
