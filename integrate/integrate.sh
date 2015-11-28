#!/bin/sh
mpicc integrate.c -o integrate.o -lm
mpiexec -n 16 -machinefile /rhome/u0356170/machinefile ./integrate.o 1023999
