#!/usr/bin/env python3
# -*- coding: utf-8 -*-

stack = []
tries = 0
found = False

def fit(sudoku, i, j, n):
    global tries
    tries += 1
    for k in range(9):
        if sudoku[i][k] == n or sudoku[k][j] == n:
            return False
    i -= i % 3
    j -= j % 3
    for k in range(i, i+3):
        for l in range(j, j+3):
            if sudoku[k][l] == n:
                return False
    return True

def recur(sudoku, d):
    global stack
    global found
    if d == len(stack):
        log(sudoku)
        found = True
    else:
        i = stack[d][0]
        j = stack[d][1]
        for n in range(9, 0, -1):
            if (fit(sudoku, i, j, n)):
                sudoku[i][j] = n
                recur(sudoku, d+1)
                sudoku[i][j] = 0

def solve(sudoku):
    global stack
    global tries
    for i in range(9):
        for j in range(9):
            if sudoku[i][j] == 0:
                stack.append((i, j))
    print('given numbers: %d\n' % (81-len(stack)))
    recur(sudoku, 0)
    if not found:
        print('no solution')
    print('total tries: %d' % tries)

def check(sudoku):
    if len(sudoku) != 9:
        raise ValueError('a sudoku must be 9 x 9')
    for line in sudoku:
        if len(line) != 9:
            raise ValueError('a sudoku must be 9 x 9')
        for n in line:
            if not isinstance(n, int):
                raise TypeError('sudoku can only contain integers')

def log(sudoku):
    buf = []
    for i in range(9):
        for j in range(9):
            buf.append(' ' if sudoku[i][j] == 0 else str(sudoku[i][j]))
            buf.append('|' if j == 2 or j == 5 else ' ')
        buf.append('\n-----+-----+-----\n' if i == 2 or i == 5 else '\n')
    print(''.join(buf))

def readfile(filename):
    with open(filename, 'r') as f:
        return [[int(n) for n in line.split()] for line in f.readlines()]

def read():
    return [[int(n) for n in input().split()] for line in range(9)]

if __name__ == '__main__':
    sudoku = [
        [2, 0, 0, 4, 7, 0, 8, 0, 0],
        [0, 0, 0, 0, 6, 8, 0, 0, 0],
        [0, 0, 0, 0, 0, 5, 6, 0, 7],
        [8, 0, 0, 0, 0, 0, 7, 3, 0],
        [1, 9, 0, 0, 4, 0, 0, 8, 2],
        [0, 2, 5, 0, 0, 0, 0, 0, 6],
        [5, 0, 9, 3, 0, 0, 0, 0, 0],
        [0, 0, 0, 8, 9, 0, 0, 0, 0],
        [0, 0, 3, 0, 1, 7, 0, 0, 4]
    ]
    sudoku = read()
    check(sudoku)
    log(sudoku)
    solve(sudoku)
