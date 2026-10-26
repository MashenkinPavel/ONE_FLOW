#cmake_minimum_required(VERSION 3.14)
#set(CMAKE_BUILD_TYPE Debug) #Release
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_C_COMPILER "mpicc")
#set(CMAKE_CXX_COMPILER "mpicxx")

# set root of location to find PETSc's pkg-config

#set(PETSC $ENV{PETSC_DIR}/$ENV{PETSC_ARCH})
#set(ENV{PKG_CONFIG_PATH} ${PETSC}/lib/pkgconfig)

#project(ONE_FLOW)
#include_directories(.)





add_executable( ONE_FLOW
				main.cpp       
				)

#find_package(PkgConfig REQUIRED)
#pkg_search_module(PETSC REQUIRED IMPORTED_TARGET PETSc)
#target_link_libraries(ONE_FLOW PkgConfig::PETSC)

#set(PTSCOTCHDIR "${PROJECT_SOURCE_DIR}/LIBS/PT_SCOTCH")
#target_include_directories(ONE_FLOW PUBLIC "${PTSCOTCHDIR}/include")

#target_link_libraries(ONE_FLOW "${PTSCOTCHDIR}/lib/libscotcherr.a")
#target_link_libraries(ONE_FLOW "${PTSCOTCHDIR}/lib/libscotch.a")

#target_link_libraries(ONE_FLOW "${PTSCOTCHDIR}/lib/libptscotch.a")
#target_link_libraries(ONE_FLOW "${PTSCOTCHDIR}/lib/libptscotchparmetisv3.a")

#target_link_libraries(ONE_FLOW "${PTSCOTCHDIR}/lib/libptscotcherr.a")








