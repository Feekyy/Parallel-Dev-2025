all:
	cls
	gcc matrix_main.c ../kernel/kernel_loader.c ../kernel/error.c -o calc.exe -lOpencl -I../kernel
clean:
	del calc.exe