CC= g++
OPTIONS= -c -O2 -m64 -march=native -std=c++11 -w

all: Main
Main: Main.o Functions.o Parser.o Classes.o
	$(CC) -o Main main.o functions.o parser.o classes.o -lm

Main.o: main.cpp parser.h functions.h classes.cpp
	$(CC) $(OPTIONS) main.cpp 

Functions.o: functions.cpp functions.h classes.cpp
	$(CC) $(OPTIONS) functions.cpp 

Parser.o: parser.cpp parser.h classes.cpp
	$(CC) $(OPTIONS) parser.cpp  

Classes.o: classes.cpp 
	$(CC) $(OPTIONS) classes.cpp 

clean:
	rm *.o Main -f

#./Main instances/Pa280_n837_bounded-strongly-corr_07.ttp --IR_GRASP 10 --SIZE_RCL 0.3 --seed 18
