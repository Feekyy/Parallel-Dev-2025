all:
	del main.exe
	cls
	gcc -Iinclude/ src/main.c src/generate.c src/sorting.c src/parallel.c -o main.exe -lOpenCL -g
	main.exe

clean:
	del main.exe
	cls

debug:
	del main.exe
	cls
	gcc -Iinclude/ src/main.c src/generate.c src/sorting.c src/parallel.c -o main.exe -lOpenCL -g
	gdb main.exe

run:
	cls
	main.exe