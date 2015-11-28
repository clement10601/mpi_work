#!/bin/sh
mpicc prime.c -o prime.o -lm
mpiexec -n 16 -machinefile /rhome/u0356170/machinefile ./prime.o 1023
