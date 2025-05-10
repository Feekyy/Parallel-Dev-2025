all:
	mkdir build
	cmake -S . -B build
	cmake --build build
	clear

refresh:
	clear
	cmake --build build

clean:
	rm -rf build
	clear

runapp:
	clear
	$(MAKE) -C build
	./build/app.exe

runtest:
	clear
	$(MAKE) -C build
	./build/test_sort.exe
