//
// Created by pavel on 03.05.2023.
//
#include "SLAE.h"
int testfunc1(int argc, char*argv[])
{

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
    PetscCall(PetscInitialize(&argc, &argv, (char *)0, help));
    PetscCallMPI(MPI_Comm_size(PETSC_COMM_WORLD, &size));
    PetscCheck(size == 1, PETSC_COMM_WORLD, PETSC_ERR_WRONG_MPI_SIZE, "This is a uniprocessor example only!");

    PetscCall(PetscOptionsGetInt(NULL, NULL, "-n", &n, NULL));

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Compute the matrix and right-hand-side vector that define
    the linear system, Ax = b.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    /*
    Create vectors. Note that we form 1 vector from scratch and
    then duplicate as needed.
    */
    PetscCall(VecCreate(PETSC_COMM_SELF, &x));
    PetscCall(PetscObjectSetName((PetscObject)x, "Solution"));
    PetscCall(VecSetSizes(x, PETSC_DECIDE, n));
    PetscCall(VecSetFromOptions(x));
    PetscCall(VecDuplicate(x, &b));
    PetscCall(VecDuplicate(x, &u));

        /*
    Create matrix. When using MatCreate(), the matrix format can
    be specified at runtime.
    Performance tuning note: For problems of substantial size,
    preallocation of matrix memory is crucial for attaining good
    performance. See the matrix chapter of the users manual for details.
    */
    PetscCall(MatCreate(PETSC_COMM_SELF, &A));
    PetscCall(MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, n, n));
    PetscCall(MatSetFromOptions(A));
    PetscCall(MatSetUp(A));

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
        PetscCall(MatSetValues(A, 1, &i, 3, col, value, INSERT_VALUES));
    }
    i = n - 1;

    col[0] = n - 2;
    col[1] = n - 1;
    PetscCall(MatSetValues(A, 1, &i, 2, col, value, INSERT_VALUES));
    i = 0;
    col[0] = 0;
    col[1]= 1;
    value[0] = 2.0;
    value[1] = -1.0;
    PetscCall(MatSetValues(A, 1, &i, 2, col, value, INSERT_VALUES));
    PetscCall(MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY));
    PetscCall(MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY));
/*
Set exact solution; then compute right-hand-side vector.
*/
    PetscCall(VecSet(u, 1.0));
    PetscCall(MatMult(A, u, b));
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Create the linear solver and set various options
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    PetscCall(KSPCreate(PETSC_COMM_SELF, &ksp));
/*
Set operators. Here the matrix that defines the linear system
also serves as the matrix that defines the preconditioner.
*/
    PetscCall(KSPSetOperators(ksp, A, A));
/*
Set linear solver defaults for this problem (optional).
- By extracting the KSP and PC contexts from the KSP context,
we can then directly call any KSP and PC routines to set
various options.
- The following four statements are optional; all of these
parameters could alternatively be specified at runtime via
KSPSetFromOptions();
*/
    PetscCall(KSPGetPC(ksp, &pc));
    PetscCall(PCSetType(pc, PCJACOBI));
    PetscCall(KSPSetTolerances(ksp, 1.e-5, PETSC_DEFAULT, PETSC_DEFAULT, PETSC_DEFAULT));
/*
Set runtime options, e.g.,
-ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
These options will override those specified above as long as
KSPSetFromOptions() is called _after_ any other customization
routines.
*/
    PetscCall(KSPSetFromOptions(ksp));
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Solve the linear system
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    PetscCall(KSPSolve(ksp, b, x));

/*
View solver info; we could instead use the option -ksp_view to
print this info to the screen at the conclusion of KSPSolve().
*/
    PetscCall(KSPView(ksp, PETSC_VIEWER_STDOUT_SELF));
    

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Check the solution and clean up
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    PetscCall(VecAXPY(x, -1.0, u));
    PetscCall(VecNorm(x, NORM_2, &norm));
    PetscCall(KSPGetIterationNumber(ksp, &its));
    PetscCall(PetscPrintf(PETSC_COMM_SELF, "Norm of error %g, Iterations %" PetscInt_FMT "\n", (double)norm, its));
    /* check that KSP automatically handles the fact that the the new non-zero values␣
    in the matrix are propagated to the KSP solver */
    PetscCall(MatShift(A, 2.0));
    PetscCall(KSPSolve(ksp, b, x));
    /*
    Free work space. All PETSc objects should be destroyed when they
    are no longer needed.
    */
    PetscCall(VecDestroy(&x));
    PetscCall(VecDestroy(&u));
    PetscCall(VecDestroy(&b));
    PetscCall(MatDestroy(&A));
    PetscCall(KSPDestroy(&ksp));
    /*
    Always call PetscFinalize() before exiting a program. This routine
    - finalizes the PETSc libraries as well as MPI
    - provides summary and diagnostic information if certain runtime
    options are chosen (e.g., -log_view).
    */
    PetscCall(PetscFinalize());
    return 1; 
}



int testfunc2(int argc, char* argv[]){
    //test 1_d
    double L=100.0;
    int Nx = 100.0;
    double dx = L/Nx;
    PetscInitialize(&argc,&argv,NULL,NULL);
    PetscInt myint;
    PetscBool set;
    PetscOptionsGetInt(NULL,NULL,"-myint",&myint,&set);
    PetscPrintf(PETSC_COMM_WORLD,"HELLO PETSc\n");
    PetscInt M=Nx;
    PetscInt m;
    PetscSplitOwnership(PETSC_COMM_WORLD,&m,&M);


    Mat A;
    Vec x,b;





    PetscFinalize();
}