# LBYARCH-MCO

HOW TO RUN THE PROGRAM:
nasm -f win64 accel.asm -o accel.o
gcc -o accel -mconsole main.c accel.o
accel.exe

INPUT:
3
0.0,62.5,10.1
60.0,122.3,5.5
30.0,160.7,7.8

EXPECTED OUTPUT:
Read data:
0.0, 62.5, 10.1
60.0, 122.3, 5.5
30.0, 160.7, 7.8
Accelerations (m/s²):
1
17
15

Correctness check:
Car 1: ASM=1, C=1 (diff=0)
Car 2: ASM=17, C=17 (diff=0)
Car 3: ASM=15, C=15 (diff=0)
Y=10: Average time = 0.000001 seconds
Y=100: Average time = 0.000008 seconds
Y=1000: Average time = 0.000078 seconds
Y=10000: Average time = 0.000782 seconds

ACTUAL OUTPUT RIGHT NOW: 
Accelerations (m/s▓):
2
3
5

Correctness check:
Car 1: ASM=2, C=1 (diff=1)
Car 2: ASM=3, C=3 (diff=0)
Car 3: ASM=5, C=4 (diff=1)
Y=10: Average time = 0.000000 seconds
Y=100: Average time = 0.000000 seconds
