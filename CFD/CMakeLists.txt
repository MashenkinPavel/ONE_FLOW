add_library(ONE_CFD STATIC 
${CMAKE_CURRENT_SOURCE_DIR}/src/ISolver.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/CSimple.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/DataRegistry.cpp

${CMAKE_CURRENT_SOURCE_DIR}/include/ISolver.h
${CMAKE_CURRENT_SOURCE_DIR}/include/CSimple.h
${CMAKE_CURRENT_SOURCE_DIR}/include/DataRegistry.h
)

target_include_directories(ONE_CFD PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ONE_CFD PRIVATE ONE_MESH)


#enable_testing()
#add_subdirectory(tests)
