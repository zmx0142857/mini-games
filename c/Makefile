CXX = g++
CC = gcc
IDIR = include
CFLAG = -I$(IDIR)

# $@ 表示目标
# $^ 表示所有依赖
bin/24points: 24points/24points.c
	mkdir -p bin
	$(CC) -o $@ $^

bin/add2n: add2n/add2n.c
	mkdir -p bin
	$(CC) -o $@ $^

bin/alarm: alarm/alarm.cpp
	mkdir -p bin
	$(CXX) -o $@ $^

bin/cube-font: cube-font/cube-font.cpp
	mkdir -p bin
	$(CXX) -o $@ $^

bin/digits: digits/digits.c
	mkdir -p bin
	$(CC) -o $@ $^

bin/go: go/*.c include/game.h
	mkdir -p bin
	$(CC) $(CFLAG) -o $@ go/*.c

bin/morse: morse/morse.cpp
	mkdir -p bin
	$(CXX) -o $@ $^

bin/rubik-cube: rubik-cube/rubik-cube.cpp
	mkdir -p bin
	$(CXX) $(CFLAG) -o $@ $^

bin/siege/siege: siege/siege.c
	mkdir -p bin/siege
	$(CC) $(CFLAG) -o $@ $^
	cp -r siege/level bin/siege

bin/snake: snake/snake.c
	mkdir -p bin
	$(CC) $(CFLAG) -o $@ $^

bin/sokoban/sokoban: sokoban/*.c include/game.h
	mkdir -p bin/sokoban
	$(CC) $(CFLAG) -o $@ sokoban/*.c
	cp -r sokoban/level bin/sokoban

bin/sphere: sphere/sphere.c
	mkdir -p bin
	$(CC) -o $@ $^ -lm

bin/tetris: tetris/tetris.c
	mkdir -p bin
	$(CC) $(CFLAG) -o $@ $^

bin/typing: typing/typing.cpp
	mkdir -p bin
	$(CXX) $(CFLAG) -o $@ $^

bin/lightoff/lightoff: lightoff/main.c
	mkdir -p bin/lightoff
	$(CC) $(CFLAG) -o $@ $^
	cp lightoff/test.txt bin/lightoff
