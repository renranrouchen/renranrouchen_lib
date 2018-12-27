objects=webserver.o
webserver:$(objects)
	gcc -o webserver $(objects)
webserver.o:webserver.c webserver.h webstring.h 

clean:
	rm -rf webserver.o webserver 

