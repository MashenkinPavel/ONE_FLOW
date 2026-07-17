//
// Created by pavel on 03.05.2023.
//

#include <gtest/gtest.h>
#include <ostream>
#include <memory>
#include <petscksp.h>

int gl_argc;
char ** gl_argv;


TEST(PETScTest, 1d_poisson){
    Vec x, b, u; /* approx solution, RHS, exact solution */
    Mat A;
    /* linear system matrix */
    KSP ksp;
    /* linear solver context */
    PC pc;
    /* preconditioner context */
    PetscReal norm;
    /* norm of solution error */
    PetscInt i, n = 10, col[3], its;
    PetscMPIInt size;
    PetscScalar value[3];
    PetscFunctionBeginUser;
    PetscInitialize(&gl_argc, &gl_argv, (char *)0, NULL);
    MPI_Comm_size(PETSC_COMM_WORLD, &size);
    //PetscCheck(size == 1, PETSC_COMM_WORLD, PETSC_ERR_WRONG_MPI_SIZE, "This is a uniprocessor example only!");

    PetscOptionsGetInt(NULL, NULL, "-n", &n, NULL);

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Compute the matrix and right-hand-side vector that define
    the linear system, Ax = b.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    /*
    Create vectors. Note that we form 1 vector from scratch and
    then duplicate as needed.
    */
    VecCreate(PETSC_COMM_SELF, &x);
    PetscObjectSetName((PetscObject)x, "Solution");
    VecSetSizes(x, PETSC_DECIDE, n);
    VecSetFromOptions(x);
    VecDuplicate(x, &b);
    VecDuplicate(x, &u);

        /*
    Create matrix. When using MatCreate(), the matrix format can
    be specified at runtime.
    Performance tuning note: For problems of substantial size,
    preallocation of matrix memory is crucial for attaining good
    performance. See the matrix chapter of the users manual for details.
    */
    MatCreate(PETSC_COMM_SELF, &A);
    MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, n, n);
    MatSetFromOptions(A);
    MatSetUp(A);

        /*
    Assemble matrix
    */
    value[0] = -1.0;
    value[1] = 2.0;
    value[2] = -1.0;
    for (i = 1; i < n - 1; i++) {
        col[0] = i - 1;
        col[1] = i;
        col[2] = i + 1;
        MatSetValues(A, 1, &i, 3, col, value, INSERT_VALUES);
    }
    i = n - 1;

    col[0] = n - 2;
    col[1] = n - 1;
    MatSetValues(A, 1, &i, 2, col, value, INSERT_VALUES);
    i = 0;
    col[0] = 0;
    col[1]= 1;
    value[0] = 2.0;
    value[1] = -1.0;
    MatSetValues(A, 1, &i, 2, col, value, INSERT_VALUES);
    MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
/*
Set exact solution; then compute right-hand-side vector.
*/
    VecSet(u, 1.0);
    MatMult(A, u, b);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Create the linear solver and set various options
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    KSPCreate(PETSC_COMM_SELF, &ksp);
/*
Set operators. Here the matrix that defines the linear system
also serves as the matrix that defines the preconditioner.
*/
    KSPSetOperators(ksp, A, A);
/*
Set linear solver defaults for this problem (optional).
- By extracting the KSP and PC contexts from the KSP context,
we can then directly call any KSP and PC routines to set
various options.
- The following four statements are optional; all of these
parameters could alternatively be specified at runtime via
KSPSetFromOptions();
*/
    KSPGetPC(ksp, &pc);
    PCSetType(pc, PCJACOBI);
    KSPSetTolerances(ksp, 1.e-5, PETSC_DEFAULT, PETSC_DEFAULT, PETSC_DEFAULT);
/*
Set runtime options, e.g.,
-ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
These options will override those specified above as long as
KSPSetFromOptions() is called _after_ any other customization
routines.
*/
    KSPSetFromOptions(ksp);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Solve the linear system
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    KSPSolve(ksp, b, x);

/*
View solver info; we could instead use the option -ksp_view to
print this info to the screen at the conclusion of KSPSolve().
*/
    KSPView(ksp, PETSC_VIEWER_STDOUT_SELF);
    

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Check the solution and clean up
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    VecAXPY(x, -1.0, u);
    VecNorm(x, NORM_2, &norm);
    KSPGetIterationNumber(ksp, &its);
    PetscPrintf(PETSC_COMM_SELF, "Norm of error %g, Iterations %" PetscInt_FMT "\n", (double)norm, its);
    /* check that KSP automatically handles the fact that the the new non-zero values␣
    in the matrix are propagated to the KSP solver */
    MatShift(A, 2.0);
    KSPSolve(ksp, b, x);
    /*
    Free work space. All PETSc objects should be destroyed when they
    are no longer needed.
    */
    VecDestroy(&x);
    VecDestroy(&u);
    VecDestroy(&b);
    MatDestroy(&A);
    KSPDestroy(&ksp);
    /*
    Always call PetscFinalize() before exiting a program. This routine
    - finalizes the PETSc libraries as well as MPI
    - provides summary and diagnostic information if certain runtime
    options are chosen (e.g., -log_view).
    */
   PetscFinalize();
    EXPECT_TRUE(4+4 == 8);
}

// int testfunc1(int argc, char*argv[])
// {

// }
// int testfunc2(int argc, char* argv[]){
//     //test 1_d
//     double L=100.0;
//     int Nx = 100.0;
//     double dx = L/Nx;
//     PetscInitialize(&argc,&argv,NULL,NULL);
//     PetscInt myint;
//     PetscBool set;
//     PetscOptionsGetInt(NULL,NULL,"-myint",&myint,&set);
//     PetscPrintf(PETSC_COMM_WORLD,"HELLO PETSc\n");
//     PetscInt M=Nx;
//     PetscInt m;
//     PetscSplitOwnership(PETSC_COMM_WORLD,&m,&M);
//     Mat A;
//     Vec x,b;
//     PetscFinalize();
//     return 0;
// }
// int testfunc3(int argc, char* argv[]){
//     Vec x, b, u; /* approx solution, RHS, exact solution */
//     Mat A; /* linear system matrix */
//     //PetscInitialize( &argc, &argv, 0, 0 );
//     //PetscPrintf( PETSC_COMM_WORLD, "Hello World\n" );
//     int rank,size;
//     PetscCallMPI(MPI_Comm_size(PETSC_COMM_WORLD, &size));
//     MPI_Comm_rank( PETSC_COMM_WORLD, &rank );
//     PetscSynchronizedPrintf( PETSC_COMM_WORLD,
//                              "Hello World from rank %d\n", rank );
//     //MatSetSizes(Mat A,PetscInt m,PetscInt n,PETSC_DETERMINE,PETSC_DETERMINE);
//     MatCreate(PETSC_COMM_WORLD,&A);
//     VecCreate(PETSC_COMM_WORLD,&x);
//     VecCreate(PETSC_COMM_WORLD,&b);
//     int m,n;
//     MatSetSizes(A,m,n,PETSC_DETERMINE,PETSC_DETERMINE);
//     MatSetType(A,MATAIJ);
//     //MatSetValues(Mat A,PetscInt m,PetscInt *im,PetscInt n,PetscInt *in,PetscScalar *values,INSERT_VALUES);
//     MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
//     MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
//     //After creating the matrix and vectors, solver creation
//     Mat Pmat;
//     MatCreate(PETSC_COMM_WORLD,&Pmat);
//     KSP ksp;
//     KSPCreate(PETSC_COMM_WORLD,&ksp);
//     KSPSetOperators( ksp,A,Pmat);
//     KSPSetFromOptions(ksp);
//     KSPSolve(ksp,b,x);
//     KSPDestroy(&ksp);
//     return 0;
// }




int main(int argc, char **argv) {
    // 1. Инициализация GoogleTest (потребляет свои флаги)
    gl_argc = argc; 
    gl_argv = argv;
    ::testing::InitGoogleTest(&argc, argv);

    // 3. Запуск всех тестов
    int result = RUN_ALL_TESTS();

    // 4. Завершение PETSc
    PetscFinalize();

    return result;
}