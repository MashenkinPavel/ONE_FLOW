




int main ( int argc, char * argv[]) {
    MPI_Comm            proccomm;
    int                 procglbnbr;                 /* Number of processes sharing graph data */
    int                 proclocnum;                 /* Number of this process                 */
    SCOTCH_Num          vertglbnbr;
    SCOTCH_Num          vertlocnbr;
    SCOTCH_Num *        fronloctab;
    SCOTCH_Num          baseval;
    SCOTCH_Dgraph       grafdat;
    SCOTCH_Dgraph       bandgrafdat;
    SCOTCH_Num          bandvertglbnbr;
    SCOTCH_Num          bandvertlocnbr;
    SCOTCH_Num *        bandvlblloctab;
    FILE *              file;
    int                 procnum;
#ifdef SCOTCH_PTHREAD
    int                 thrdreqlvl;
  int                 thrdprolvl;
#endif /* SCOTCH_PTHREAD */

    SCOTCH_errorProg (argv[0]);

#ifdef SCOTCH_PTHREAD
    thrdreqlvl = MPI_THREAD_MULTIPLE;
  if (MPI_Init_thread (&argc, &argv, thrdreqlvl, &thrdprolvl) != MPI_SUCCESS)
    SCOTCH_errorPrint ("main: Cannot initialize (1)");
#else /* SCOTCH_PTHREAD */
    if (MPI_Init (&argc, &argv) != MPI_SUCCESS)
        SCOTCH_errorPrint ("main: Cannot initialize (2)");
#endif /* SCOTCH_PTHREAD */

    if (argc != 2) {
        SCOTCH_errorPrint ("usage: %s graph_file", argv[0]);
        exit (EXIT_FAILURE);
    }

    proccomm = MPI_COMM_WORLD;
    MPI_Comm_size (proccomm, &procglbnbr);          /* Get communicator data */
    MPI_Comm_rank (proccomm, &proclocnum);

#ifdef SCOTCH_CHECK_NOAUTO
    fprintf (stderr, "Proc %2d of %2d, pid %d\n", proclocnum, procglbnbr, getpid ());

  if (proclocnum == 0) {                          /* Synchronize on keybord input */
    char           c;

    printf ("Waiting for key press...\n");
    scanf ("%c", &c);
  }
#endif /* SCOTCH_CHECK_NOAUTO */

    if (MPI_Barrier (proccomm) != MPI_SUCCESS) {    /* Synchronize for debug */
        SCOTCH_errorPrint ("main: cannot communicate (1)");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_dgraphInit (&grafdat, proccomm) != 0) { /* Initialize source graph */
        SCOTCH_errorPrint ("main: cannot initialize graph (1)");
        exit (EXIT_FAILURE);
    }

    file = NULL;
    if ((proclocnum == 0) &&
        ((file = fopen (argv[1], "r")) == NULL)) {
        SCOTCH_errorPrint ("main: cannot open graph file");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_dgraphLoad (&grafdat, file, -1, 0) != 0) {
        SCOTCH_errorPrint ("main: cannot load graph");
        exit (EXIT_FAILURE);
    }

    if (file != NULL)
        fclose (file);

    if (MPI_Barrier (proccomm) != MPI_SUCCESS) {    /* Synchronize for debug */
        SCOTCH_errorPrint ("main: cannot communicate (2)");
        exit (EXIT_FAILURE);
    }

    SCOTCH_dgraphData (&grafdat, &baseval, &vertglbnbr, &vertlocnbr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if ((fronloctab = (SCOTCH_Num*) malloc (vertlocnbr * sizeof (SCOTCH_Num))) == NULL) {
        SCOTCH_errorPrint ("main: cannot allocate frontier array");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_dgraphInit (&bandgrafdat, proccomm) != 0) { /* Initialize band graph */
        SCOTCH_errorPrint ("main: cannot initialize graph (2)");
        exit (EXIT_FAILURE);
    }

    fronloctab[0] = baseval;

    if (SCOTCH_dgraphBand (&grafdat, (proclocnum == 1) ? 1 : 0, fronloctab, 4, &bandgrafdat) != 0) {
        SCOTCH_errorPrint ("main: cannot compute band graph");
        exit (EXIT_FAILURE);
    }

    free (fronloctab);

    SCOTCH_dgraphData (&bandgrafdat, &baseval, &bandvertglbnbr, &bandvertlocnbr, NULL, NULL, NULL, NULL, NULL, &bandvlblloctab, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    for (procnum = 0; procnum < procglbnbr; procnum ++) {
        SCOTCH_Num          bandvertlocnum;

        MPI_Barrier (proccomm);

        if (procnum == proclocnum) {
            if ((file = fopen ("/tmp/test_scotch_dgraph_band.map", (procnum == 0) ? "w" : "a+")) == NULL) {
                SCOTCH_errorPrint ("main: cannot open mapping file");
                exit (EXIT_FAILURE);
            }

            if (procnum == 0)
                fprintf (file, "%ld\n", (long) bandvertglbnbr);

            for (bandvertlocnum = 0; bandvertlocnum < bandvertlocnbr; bandvertlocnum ++)
                fprintf (file, "%ld\t1\n", (long) bandvlblloctab[bandvertlocnum]);

            fclose (file);
        }
    }

    SCOTCH_dgraphExit (&bandgrafdat);
    SCOTCH_dgraphExit (&grafdat);

    MPI_Finalize ();
    exit (EXIT_SUCCESS);
}




void test_scotch_arch_c(int argc, char* argv[]){

    FILE *              fileptr;
    SCOTCH_Arch         archtab[ARCHNBR];
    SCOTCH_Num          vnumnbr = 5;
    SCOTCH_Num          vnumtab[8] = { 0, 4, 1, 5, 7, 11, 13, 15 };
    SCOTCH_Num          dimnnbr = 5;
    SCOTCH_Num          dimntab[5] = { 3, 3, 5, 4, 8 };
    SCOTCH_Num          levlnbr = 3;
    SCOTCH_Num          sizetab[3] = { 6, 3, 4 };
    SCOTCH_Num          linktab[3] = { 20, 5, 1 };
    int                 archnbr = 0;
    int                 i;

    SCOTCH_errorProg (argv[0]);

    SCOTCH_randomReset ();

    if (SCOTCH_archHcub (&archtab[archnbr ++], 4) != 0) { /* TRICK: must be in rank 0 to create sub-architecture from it */
        SCOTCH_errorPrint ("main: cannot create hcub architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archCmplt (&archtab[archnbr ++], 8) != 0) {
        SCOTCH_errorPrint ("main: cannot create cmplt architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archMesh2 (&archtab[archnbr ++], 2, 4) != 0) {
        SCOTCH_errorPrint ("main: cannot create mesh2D architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archMesh3 (&archtab[archnbr ++], 3, 4, 5) != 0) {
        SCOTCH_errorPrint ("main: cannot create mesh3D architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archMeshX (&archtab[archnbr ++], dimnnbr, dimntab) != 0) {
        SCOTCH_errorPrint ("main: cannot create meshXD architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archTleaf (&archtab[archnbr ++], levlnbr, sizetab, linktab) != 0) {
        SCOTCH_errorPrint ("main: cannot create tleaf architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archTorus2 (&archtab[archnbr ++], 2, 4) != 0) {
        SCOTCH_errorPrint ("main: cannot create torus2D architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archTorus3 (&archtab[archnbr ++], 3, 4, 5) != 0) {
        SCOTCH_errorPrint ("main: cannot create torus3D architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archTorusX (&archtab[archnbr ++], dimnnbr, dimntab) != 0) {
        SCOTCH_errorPrint ("main: cannot create torusXD architecture");
        exit (EXIT_FAILURE);
    }

    if (SCOTCH_archSub (&archtab[archnbr ++], &archtab[0], vnumnbr + 3, vnumtab) != 0) { /* TRICK: create sub-architecture of hypercube at rank 0 */
        SCOTCH_errorPrint ("main: cannot create sub-architecture (1)");
        exit (EXIT_FAILURE);
    }

    for (i = 0; i < archnbr; i ++) {
        if (SCOTCH_archSub (&archtab[i + archnbr], &archtab[i], vnumnbr, vnumtab) != 0) { /* Create sub-architectures (of sub-architecture, too) */
            SCOTCH_errorPrint ("main: cannot create sub-architecture (%d)", 2 + i);
            exit (EXIT_FAILURE);
        }
    }
    archnbr *= 2;                                   /* Number of architectures has doubled */

    if ((fileptr = fopen (argv[1], "w+")) == NULL) { /* Write all architectures to file */
        SCOTCH_errorPrint ("main: cannot open file (1)");
        exit (EXIT_FAILURE);
    }

    for (i = 0; i < archnbr; i ++) {                /* Save all architectures to same file */
        if (SCOTCH_archSave (&archtab[i], fileptr) != 0) {
            SCOTCH_errorPrint ("main: cannot save architecture (%d)", 1 + i);
            exit (EXIT_FAILURE);
        }
    }

    fclose (fileptr);

    for (i = archnbr - 1; i >= 0; i --)             /* Destroy architectures in reverse order to destroy sub-architectures first */
        SCOTCH_archExit (&archtab[i]);

    if ((fileptr = fopen (argv[1], "r")) == NULL) { /* Read all architectures from file where they were written to */
        SCOTCH_errorPrint ("main: cannot open file (2)");
        exit (EXIT_FAILURE);
    }

    for (i = 0; i < archnbr; i ++) {
        if ((SCOTCH_archInit (&archtab[i])          != 0) ||
            (SCOTCH_archLoad (&archtab[i], fileptr) != 0)) {
            SCOTCH_errorPrint ("main: cannot load architecture (%d)", 1 + i);
            exit (EXIT_FAILURE);
        }
    }

    fclose (fileptr);

    for (i = 0; i < archnbr; i ++)                  /* Destroy architectures in any order, as they are now all autonomous from each other */
        SCOTCH_archExit (&archtab[i]);

    exit (EXIT_SUCCESS);



}