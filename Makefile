all: display stat watershed
display: display.o pointcloud.o util.o bmp.o
	gcc -g display.o pointcloud.o util.o bmp.o -o display -lm

display.o: display.c pointcloud.h util.h bmp.h
	gcc -g -c display.c

pointcloud.o: pointcloud.c pointcloud.h bmp.h
	gcc -g -c pointcloud.c

util.o: util.c util.h
	gcc -g -c util.c

bmp.o: bmp.c bmp.h
	gcc -g -c bmp.c

stat: pointcloud.o parta.o bmp.o util.o
	gcc -g pointcloud.o parta.o bmp.o util.o -o stat -lm

watershed: watershed.o pointcloud.o util.o bmp.o
	gcc -g watershed.o pointcloud.o util.o bmp.o -o watershed -lm

watershed.o: watershed.c pointcloud.h util.h bmp.h
	gcc -g -c watershed.c

clean: 
	rm *.o stat display watershed out.gif