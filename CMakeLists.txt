cmake_minimum_required(VERSION 3.9)
project(NGP C)

set(EXEC Devillez_Poncelet_Fortune) # put whatever name you want for the executable 

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(${EXEC}
               "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/inputs.c"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/halfEdge.c"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/events.c"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/binaryTree.c"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/circle.c"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/queue.c"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/utils.c"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/fortune.c"
               # you can add other source file here !
               )

target_include_directories(${EXEC} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CMAKE_C_FLAGS "-O3")
set_target_properties(${EXEC} PROPERTIES
                      C_STANDARD 99
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

add_subdirectory(deps/BOV)
target_link_libraries(${EXEC} bov)

file(MAKE_DIRECTORY "video")

# set ${EXEC} as the startup project in visual studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXEC})
