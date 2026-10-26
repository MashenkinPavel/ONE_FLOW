#cmake_minimum_required(VERSION 3.14)
#project(mesh VERSION 1.0.0 LANGUAGES CXX)

# Устанавливаем стандарт C++ (например, C++17)
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)


#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

# Опционально: тип сборки по умолчанию
#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
#endif()

# Подключаем подпроекты
#add_subdirectory(src)

add_library(ONE_MESH STATIC 
${CMAKE_CURRENT_SOURCE_DIR}/src/BC.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/cell.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/face.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/foam_mesh.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/mesh.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/node.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/Patch.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/Rib.cpp

${CMAKE_CURRENT_SOURCE_DIR}/include/BC.h
${CMAKE_CURRENT_SOURCE_DIR}/include/cell.h
${CMAKE_CURRENT_SOURCE_DIR}/include/face.h
${CMAKE_CURRENT_SOURCE_DIR}/include/foam_mesh.h
${CMAKE_CURRENT_SOURCE_DIR}/include/mesh.h
${CMAKE_CURRENT_SOURCE_DIR}/include/node.h
${CMAKE_CURRENT_SOURCE_DIR}/include/Patch.h
${CMAKE_CURRENT_SOURCE_DIR}/include/Rib.h
)

target_include_directories(ONE_MESH PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


enable_testing()
add_subdirectory(tests)
