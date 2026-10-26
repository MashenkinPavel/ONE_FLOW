find_package(GTest REQUIRED)
add_executable(tests_mesh mesh_ut.cpp
                     mesh_it_1cell.cpp)


target_link_libraries(tests_mesh PRIVATE GTest::gtest_main)
target_link_libraries(tests_mesh PRIVATE ONE_MESH)


include(GoogleTest)
gtest_discover_tests(tests_mesh)


# Эта команда выполнится сразу после успешной сборки my_lib_tests
# add_custom_command(
#     TARGET tests_mesh
#     POST_BUILD
#     COMMAND tests_mesh
#     COMMENT "Running unit tests after build..."
# )
 
# Создает цель, которая всегда выполняется при сборке
add_custom_target(run_tests ALL
    COMMAND tests_mesh
    DEPENDS ONE_MESH
    COMMENT "Running tests..."
)


# Для удобства можно задать рабочую директорию, если тесты используют файлы
# set_target_properties(tests_mylib PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
