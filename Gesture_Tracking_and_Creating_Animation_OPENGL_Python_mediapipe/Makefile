CC = g++

all: clean install run

install:
	$(CC) src/main.cpp src/InitShader.cpp -o src/main -lGL -lglfw -lGLEW -I include/
	$(CC) src/main2.cpp src/InitShader.cpp -o src/main2 -lGL -lglfw -lGLEW -I include/
	$(CC) src/main3.cpp src/InitShader.cpp -o src/main3 -lGL -lglfw -lGLEW -I include/
clean:
	rm -f src/main
	rm -f src/main2
	rm -f src/main3

run:
	cd src && ./main
	cd src && ./main2
	cd src && ./main3