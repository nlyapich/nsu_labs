#!bin/bash

echo "" >> result.txt

echo "with_mpi1:" >> result.txt

mpicc copy_mpi1.c -o copy_mpi1 -lm

mpiexec -n 1 ./copy_mpi1 >> result.txt
mpiexec -n 2 ./copy_mpi1 >> result.txt
mpiexec -n 3 ./copy_mpi1 >> result.txt
mpiexec -n 4 ./copy_mpi1 >> result.txt

echo "with_mpi2:" >> result.txt

mpicc copy_mpi2.c -o copy_mpi2 -lm

mpiexec -n 1 ./copy_mpi2 >> result.txt
mpiexec -n 2 ./copy_mpi2 >> result.txt
mpiexec -n 3 ./copy_mpi2 >> result.txt
mpiexec -n 4 ./copy_mpi2 >> result.txt
