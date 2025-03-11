all:
	del main.exe
	cls
	gcc -Iinclude/ src/main.c src/app.c src/scene.c src/draw.c -o main.exe -lmingw32 -lSDL2main -lSDL2 -lopengl32
	main.exe
clean:
	del main.exe
	cls