#!/bin/sh
mpicc mpi.c -o mpi.o
mpiexec -n 16 -machinefile /rhome/u0356170/machinefile ./mpi.o
