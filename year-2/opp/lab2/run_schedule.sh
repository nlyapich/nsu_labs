#!bin/bash

echo "" > result_schedule.txt

echo "schedule_static:" >> result_schedule.txt

gcc schedule_static.c -o schedule_static -lm -fopenmp

OMP_NUM_THREADS=1 ./schedule_static >> result_schedule.txt
OMP_NUM_THREADS=2 ./schedule_static >> result_schedule.txt
OMP_NUM_THREADS=3 ./schedule_static >> result_schedule.txt
OMP_NUM_THREADS=4 ./schedule_static >> result_schedule.txt
OMP_NUM_THREADS=5 ./schedule_static >> result_schedule.txt
OMP_NUM_THREADS=6 ./schedule_static >> result_schedule.txt
OMP_NUM_THREADS=7 ./schedule_static >> result_schedule.txt
OMP_NUM_THREADS=8 ./schedule_static >> result_schedule.txt

echo "schedule_dynamic:" >> result_schedule.txt

gcc schedule_dynamic.c -o schedule_dynamic -lm -fopenmp

OMP_NUM_THREADS=1 ./schedule_dynamic >> result_schedule.txt
OMP_NUM_THREADS=2 ./schedule_dynamic >> result_schedule.txt
OMP_NUM_THREADS=3 ./schedule_dynamic >> result_schedule.txt
OMP_NUM_THREADS=4 ./schedule_dynamic >> result_schedule.txt
OMP_NUM_THREADS=5 ./schedule_dynamic >> result_schedule.txt
OMP_NUM_THREADS=6 ./schedule_dynamic >> result_schedule.txt
OMP_NUM_THREADS=7 ./schedule_dynamic >> result_schedule.txt
OMP_NUM_THREADS=8 ./schedule_dynamic >> result_schedule.txt

echo "schedule_guided:" >> result_schedule.txt

gcc schedule_guided.c -o schedule_guided -lm -fopenmp

OMP_NUM_THREADS=1 ./schedule_guided >> result_schedule.txt
OMP_NUM_THREADS=2 ./schedule_guided >> result_schedule.txt
OMP_NUM_THREADS=3 ./schedule_guided >> result_schedule.txt
OMP_NUM_THREADS=4 ./schedule_guided >> result_schedule.txt
OMP_NUM_THREADS=5 ./schedule_guided >> result_schedule.txt
OMP_NUM_THREADS=6 ./schedule_guided >> result_schedule.txt
OMP_NUM_THREADS=7 ./schedule_guided >> result_schedule.txt
OMP_NUM_THREADS=8 ./schedule_guided >> result_schedule.txt

echo "schedule_auto:" >> result_schedule.txt

gcc schedule_auto.c -o schedule_auto -lm -fopenmp

OMP_NUM_THREADS=1 ./schedule_auto >> result_schedule.txt
OMP_NUM_THREADS=2 ./schedule_auto >> result_schedule.txt
OMP_NUM_THREADS=3 ./schedule_auto >> result_schedule.txt
OMP_NUM_THREADS=4 ./schedule_auto >> result_schedule.txt
OMP_NUM_THREADS=5 ./schedule_auto >> result_schedule.txt
OMP_NUM_THREADS=6 ./schedule_auto >> result_schedule.txt
OMP_NUM_THREADS=7 ./schedule_auto >> result_schedule.txt
OMP_NUM_THREADS=8 ./schedule_auto >> result_schedule.txt
