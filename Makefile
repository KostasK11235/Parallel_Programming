CC=gcc
CFLAGS=-O3 -fopenmp
LDLIBS=-lm
MPICC=mpicc

#TODO: add the following cases: multistart_mds_omp multistart_mds_omp_tasks multistart_mds_mpi

all: multistart_mds_seq multistart_mds_omp multistart_mds_omp_tasks multistart_mds_mpi

# Sequential version
multistart_mds_seq: multistart_mds_seq2.c torczon.c Makefile
        $(CC) $(CFLAGS) -o multistart_mds_seq multistart_mds_seq2.c torczon.c $(LDLIBS)

# OpenMP version
multistart_mds_omp: multistart_mds_omp.c torczon.c Makefile
        $(CC) $(CFLAGS) -o multistart_mds_omp multistart_mds_omp.c torczon.c $(LDLIBS)

# OpenMP Tasks Version
multistart_mds_omp_tasks: multistart_mds_omp.c torczon_tasks.c Makefile
        $(CC) $(CFLAGS) -o multistart_mds_omp_tasks multistart_mds_omp.c torczon_tasks.c $(LDLIBS)

# MPI version
multistart_mds_mpi: multistart_mds_mpi.c torczon.c Makefile
        $(MPICC) $(CFLAGS) -o multistart_mds_mpi multistart_mds_mpi.c torczon.c $(LDLIBS)


clean:
        @echo "Cleaning the executables..."
        rm -f multistart_mds_seq multistart_mds_omp multistart_mds_omp_tasks multistart_mds_mpi