STD=--std=c++17 -pthread
GCC=g++
OBJ=obj
BIN=bin

bin/mergesort: src/main.cpp obj/mergesort.o obj/testLib.o obj/threadmerge.o obj/semaphore.o
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${GCC} ${STD} -o bin/mergesort src/main.cpp obj/mergesort.o obj/testLib.o obj/threadmerge.o obj/semaphore.o

obj/mergesort.o: src/mergesort.h src/mergesort.cpp
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${GCC} ${STD} -c -o obj/mergesort.o src/mergesort.cpp
 
obj/threadmerge.o: src/threadmerge.h src/threadmerge.cpp
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${GCC} ${STD} -c -o obj/threadmerge.o src/threadmerge.cpp

obj/testLib.o: src/testLib.h src/testLib.cpp
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${GCC} ${STD} -c -o obj/testLib.o src/testLib.cpp

obj/semaphore.o: src/semaphore.cpp src/semaphore.h
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${GCC} ${STD} -c -o obj/semaphore.o src/semaphore.cpp

run: bin/mergesort
	bin/mergesort || true

.PHONY: doc
doc:
	doxygen config
	cd latex && make

clean:
	rm -f obj/mergesort.o
	rm -f obj/threadmerge.o
	rm -f obj/testLib.o
	rm -f bin/mergesort
	rm -r -f bin
	rm -r -f obj
	rm -r -f html
	rm -r -f latex
