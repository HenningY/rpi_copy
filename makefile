CC = g++
FLAG = -O2 -lwiringPi -lwiringPiDev

# all:bin/example bin/off_example bin/breathe_example bin/color_test bin/part_test
all:pca bin/example

pca:
	mkdir -p bin obj
	g++ -O2 -c ./src/pca.cpp -I./lib -o ./obj/pca.o -lwiringPi -lwiringPiDev
	g++ -O2 -c ./src/pca995X.cpp -I./lib -o ./obj/pca995X.o -lwiringPi -lwiringPiDev
	g++ -O2 -c ./lib/pcaDefinition.h -o ./obj/pcaDefinition.o
	g++ -O2 -c ./lib/OFrgba_to_bgriref.h -o ./obj/OFrgba_to_bgriref.o
	g++ -O2 -c ./src/OF_example.cpp -I./lib -o ./obj/OF_example.o -lwiringPi -lwiringPiDev
	g++ -O2 ./obj/pca.o ./obj/pca995X.o ./obj/OF_example.o -I./lib -o ./bin/OF_example -lwiringPi -l wiringPiDev

./obj/LED_strip.o: ./src/LED_strip.cpp ./lib/LED_strip.h
	$(CC) $(FLAG) -c ./src/LED_strip.cpp -o ./obj/LED_strip.o

./obj/gamma_correction.o: ./src/gamma_correction.cpp ./lib/gamma_correction.h
	$(CC) $(FLAG) -c ./src/gamma_correction.cpp -o ./obj/gamma_correction.o

bin/example:  ./src/example.cpp  ./obj/LED_strip.o ./obj/gamma_correction.o
	$(CC) $(FLAG) ./src/example.cpp ./obj/LED_strip.o ./obj/pca.o ./obj/pca995X.o -o ./bin/example

# bin/part_example: ./obj/LED_strip.o
# 	$(CC) $(FLAG) ./src/part_example.cpp ./obj/LED_strip.o -o ./bin/part_example

# bin/off_example:./obj/LED_strip.o
# 	$(CC) $(FLAG) ./src/off_example.cpp ./obj/LED_strip.o -o ./bin/off_example

# bin/breathe_example:./obj/LED_strip.o ./obj/gamma_correction.o
# 	$(CC) $(FLAG) ./src/breathe_example.cpp ./obj/LED_strip.o ./obj/gamma_correction.o -o ./bin/breathe_example

# bin/color_test: ./obj/LED_strip.o ./obj/gamma_correction.o
# 	$(CC) $(FLAG) ./src/test_color.cpp ./obj/LED_strip.o ./obj/gamma_correction.o -o ./bin/color_test

# bin/part_test:./obj/LED_strip.o
# 	$(CC) $(FLAG) ./src/part_test.cpp ./obj/LED_strip.o -o ./bin/part_test



	
clean:
	rm -rf ./bin/*
	rm -rf ./obj/*.*
