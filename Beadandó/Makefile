all:
	cls
	del main.exe
	gcc main.c ../kernel/kernel_loader.c ../kernel/error.c -o main.exe -lOpencl -I../kernel
clean:
	del main.exe