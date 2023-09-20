cmake_minimum_required(VERSION 3.14)
set (CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpicxx")

# set root of location to find PETSc's pkg-config
set(PETSC $ENV{PETSC_DIR}/$ENV{PETSC_ARCH})
set(ENV{PKG_CONFIG_PATH} ${PETSC}/lib/pkgconfig)

# Remove the lines below if you do not wish to have PETSc determine the compilers
execute_process ( COMMAND pkg-config PETSc --variable=ccompiler COMMAND tr -d '\n' OUTPUT_VARIABLE C_COMPILER)

#SET(CMAKE_C_COMPILER ${C_COMPILER})
#SET(CMAKE_C_COMPILER "home/pavel/STORAGE/LIBS/openmpi-4.1.5/install/bin/mpicc")
execute_process ( COMMAND pkg-config PETSc --variable=cxxcompiler COMMAND tr -d '\n' OUTPUT_VARIABLE CXX_COMPILER)
#if (CXX_COMPILER)
#	SET(CMAKE_CXX_COMPILER "/home/pavel/STORAGE/LIBS/openmpi-4.1.5/install/bin/mpicxx")
   #SET(CMAKE_CXX_COMPILER ${CXX_COMPILER})
#endif (CXX_COMPILER)
execute_process ( COMMAND pkg-config PETSc --variable=fcompiler COMMAND tr -d '\n' OUTPUT_VARIABLE FORTRAN_COMPILER)
#if (FORTRAN_COMPILER)
#  SET(CMAKE_Fortran_COMPILER ${FORTRAN_COMPILER})
#  enable_language(Fortran)
#endif (FORTRAN_COMPILER)

#  tells CMake to build the application ex1 from the source file ex1.c
#  this must appear AFTER the compilers are set

project(ONE_FLOW)
include_directories(.)
add_executable( ONE_FLOW
		main.cpp
		mesh.cpp       mesh.h
		node.cpp       node.h
		face.cpp       face.h
		cell.cpp       cell.h
		Rib.cpp        Rib.h
		SLAE.cpp       SLAE.h)


find_package(PkgConfig REQUIRED)
pkg_search_module(PETSC REQUIRED IMPORTED_TARGET PETSc)
target_link_libraries(ONE_FLOW PkgConfig::PETSC)
