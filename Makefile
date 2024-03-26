all:
	cls
	del main.exe
	gcc -Wall main.c image_import.c countor.c ../kernel/kernel_loader.c ../kernel/error.c -o main.exe -lOpencl -I../kernel
clean:
	del main.exe