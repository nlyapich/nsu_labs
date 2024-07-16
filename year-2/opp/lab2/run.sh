#!bin/bash

echo "" > result.txt

echo "main:" >> result.txt

gcc main.c -o main -lm
./main >> result.txt

echo "with_omp:" >> result.txt

gcc with_omp.c -o with_omp -lm -fopenmp

OMP_NUM_THREADS=1 ./with_omp >> result.txt
OMP_NUM_THREADS=2 ./with_omp >> result.txt
OMP_NUM_THREADS=3 ./with_omp >> result.txt
OMP_NUM_THREADS=4 ./with_omp >> result.txt
OMP_NUM_THREADS=5 ./with_omp >> result.txt
OMP_NUM_THREADS=6 ./with_omp >> result.txt
OMP_NUM_THREADS=7 ./with_omp >> result.txt
OMP_NUM_THREADS=8 ./with_omp >> result.txt


echo "with_omp2:" >> result.txt

gcc with_omp2.c -o with_omp2 -lm -fopenmp

OMP_NUM_THREADS=1 ./with_omp2 >> result.txt
OMP_NUM_THREADS=2 ./with_omp2 >> result.txt
OMP_NUM_THREADS=3 ./with_omp2 >> result.txt
OMP_NUM_THREADS=4 ./with_omp2 >> result.txt
OMP_NUM_THREADS=5 ./with_omp2 >> result.txt
OMP_NUM_THREADS=6 ./with_omp2 >> result.txt
OMP_NUM_THREADS=7 ./with_omp2 >> result.txt
OMP_NUM_THREADS=8 ./with_omp2 >> result.txt
