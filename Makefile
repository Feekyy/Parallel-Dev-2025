all:
	del main.exe
	cls
	gcc -Iinclude/ src/main.c src/generate.c src/sorting.c src/opencl.c -o main.exe -lOpenCL
	main.exe
clean:
	del main.exe
	cls
debug:
	del main.exe
	cls
	gcc -Iinclude/ src/main.c src/generate.c src/sorting.c src/opencl.c -o main.exe -lOpenCL -g
	gdb main.exe