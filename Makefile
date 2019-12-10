OBJS0	= main.o DataTypes.o Utils.o Stack.o Utils_part2.o Datatypes_part2.o Vector.o List.o
SOURCE0	= main.cpp project_jj_lib/DataTypes.cpp project_jj_lib/Utils.cpp project_jj_lib/Stack.cpp project_jj_lib_part2/Utils_part2.cpp project_jj_lib_part2/Datatypes_part2.cpp project_jj_lib_part2/Vector.cpp project_jj_lib_part2/List.cpp
HEADER0	= project_jj_lib/DataTypes.h project_jj_lib/Utils.h project_jj_lib/Stack.h project_jj_lib_part2/Utils_part2.h project_jj_lib_part2/Datatypes_part2.h project_jj_lib_part2/Vector.h project_jj_lib_part2/List.h MACROS.h
OUT0	= ExecQueries

OBJS1	= AbsoluteTests.o project_jj_lib_part2/Vector.o project_jj_lib_part2/List.o
SOURCE1	= Catch_tests/AbsoluteTests.cpp project_jj_lib_part2/Vector.cpp project_jj_lib_part2/List.o
HEADER1	= project_jj_lib_part2/Vector.h project_jj_lib_part2/List.h
OUT1	= Catch_tests_run

CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: ExecQueries Catch_tests_run

ExecQueries: $(OBJS0) $(LFLAGS)
	$(CC) -g $(OBJS0) -o $(OUT0)

Catch_tests_run: $(OBJS1) $(LFLAGS)
	$(CC) -g $(OBJS1) -o $(OUT1)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++14

DataTypes.o: project_jj_lib/DataTypes.cpp
	$(CC) $(FLAGS) project_jj_lib/DataTypes.cpp -std=c++14

Utils.o: project_jj_lib/Utils.cpp
	$(CC) $(FLAGS) project_jj_lib/Utils.cpp -std=c++14

Stack.o: project_jj_lib/Stack.cpp
	$(CC) $(FLAGS) project_jj_lib/Stack.cpp -std=c++14

Datatypes_part2.o: project_jj_lib_part2/Datatypes_part2.cpp
	$(CC) $(FLAGS) project_jj_lib_part2/Datatypes_part2.cpp -std=c++14

Utils_part2.o: project_jj_lib_part2/Utils_part2.cpp
	$(CC) $(FLAGS) project_jj_lib_part2/Utils_part2.cpp -std=c++14

Vector.o: project_jj_lib_part2/Vector.cpp
	$(CC) $(FLAGS) project_jj_lib_part2/Vector.cpp -std=c++14

List.o: project_jj_lib_part2/List.cpp
	$(CC) $(FLAGS) project_jj_lib_part2/List.cpp -std=c++14

AbsoluteTests.o: Catch_tests/AbsoluteTests.cpp
	$(CC) $(FLAGS) Catch_tests/AbsoluteTests.cpp -std=c++14

clean:
	rm -rf build/
