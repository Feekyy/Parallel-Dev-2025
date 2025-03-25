all:
	del main.exe
	cls
	gcc main.c image_import.c countor.c -o main.exe
	main.exe
clean:
	del main.exe
	cls
debug:
	del main.exe
	cls
	gcc main.c image_import.c countor.c -o main.exe -g
	gdb main.exe