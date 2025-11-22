nasm -f win64 accel.asm -o accel.o
gcc -o accel main.c accel.o
accel.exe