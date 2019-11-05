OBJS	= main.o,project_jj_lib/DataTypes.o,project_jj_lib/Utils.o,project_jj_lib/LinkedList.o,project_jj_lib/Stack.o,Catch_tests/AbsoluteTests.o
OUT	= SortMergeJoin, Catch_tests_run

OBJS0	= main.o DataTypes.o Utils.o LinkedList.o Stack.o
SOURCE0	= main.cpp project_jj_lib/DataTypes.cpp project_jj_lib/Utils.cpp project_jj_lib/LinkedList.cpp project_jj_lib/Stack.cpp
HEADER0	= project_jj_lib/DataTypes.h project_jj_lib/Utils.h project_jj_lib/LinkedList.h project_jj_lib/Stack.h
OUT0	= SortMergeJoin

OBJS1	= AbsoluteTests.o DataTypes.o Utils.o LinkedList.o Stack.o
SOURCE1	= Catch_tests/AbsoluteTests.cpp project_jj_lib/DataTypes.cpp project_jj_lib/Utils.cpp project_jj_lib/LinkedList.cpp project_jj_lib/Stack.cpp
HEADER1	= project_jj_lib/DataTypes.h project_jj_lib/Utils.h project_jj_lib/LinkedList.h project_jj_lib/Stack.h
OUT1	= Catch_tests_run

CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: SortMergeJoin Catch_tests_run

SortMergeJoin: $(OBJS0) $(LFLAGS)
	$(CC) -g $(OBJS0) -o $(OUT0)

Catch_tests_run: $(OBJS1) $(LFLAGS)
	$(CC) -g $(OBJS1) -o $(OUT1)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++14

DataTypes.o: project_jj_lib/DataTypes.cpp
	$(CC) $(FLAGS) project_jj_lib/DataTypes.cpp -std=c++14

Utils.o: project_jj_lib/Utils.cpp
	$(CC) $(FLAGS) project_jj_lib/Utils.cpp -std=c++14

LinkedList.o: project_jj_lib/LinkedList.cpp
	$(CC) $(FLAGS) project_jj_lib/LinkedList.cpp -std=c++14

Stack.o: project_jj_lib/Stack.cpp
	$(CC) $(FLAGS) project_jj_lib/Stack.cpp -std=c++14

AbsoluteTests.o: Catch_tests/AbsoluteTests.cpp
	$(CC) $(FLAGS) Catch_tests/AbsoluteTests.cpp -std=c++14


clean:
	rm -rf build/
