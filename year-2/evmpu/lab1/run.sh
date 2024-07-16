#!bin/bash

echo "" > test.txt

echo "Без оптимизаций" >> test.txt

gcc -o lab1.out lab1.c -lrt
./lab1.out 5000000000 >> test.txt
./lab1.out 10000000000 >> test.txt
./lab1.out 15000000000 >> test.txt

echo "С оптимизацией -O0:" >> test.txt

gcc -O0 -o lab1_O0.out lab1.c -lrt
./lab1_O0.out 5000000000 >> test.txt
./lab1_O0.out 10000000000 >> test.txt
./lab1_O0.out 15000000000 >> test.txt

echo "С оптимизацией -O1:" >> test.txt

gcc -O1 -o lab1_O1.out lab1.c -lrt
./lab1_O1.out 5000000000 >> test.txt
./lab1_O1.out 10000000000 >> test.txt
./lab1_O1.out 15000000000 >> test.txt

echo "С оптимизацией -O2:" >> test.txt

gcc -O2 -o lab1_O2.out lab1.c -lrt
./lab1_O2.out 5000000000 >> test.txt
./lab1_O2.out 10000000000 >> test.txt
./lab1_O2.out 15000000000 >> test.txt

echo "С оптимизацией -O3:" >> test.txt

gcc -O3 -o lab1_O3.out lab1.c -lrt
./lab1_O3.out 5000000000 >> test.txt
./lab1_O3.out 10000000000 >> test.txt
./lab1_O3.out 15000000000 >> test.txt

echo "С оптимизацией -Os:" >> test.txt

gcc -Os -o lab1_Os.out lab1.c -lrt
./lab1_Os.out 5000000000 >> test.txt
./lab1_Os.out 10000000000 >> test.txt
./lab1_Os.out 15000000000 >> test.txt

echo "С оптимизацией -Ofast:" >> test.txt

gcc -Ofast -o lab1_Ofast.out lab1.c -lrt
./lab1_Ofast.out 5000000000 >> test.txt
./lab1_Ofast.out 10000000000 >> test.txt
./lab1_Ofast.out 15000000000 >> test.txt

echo "С оптимизацией -Og:" >> test.txt

gcc -Og -o lab1_Og.out lab1.c -lrt
./lab1_Og.out 5000000000 >> test.txt
./lab1_Og.out 10000000000 >> test.txt
./lab1_Og.out 15000000000 >> test.txt
