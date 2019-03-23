#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""tic tac toe"""

class Board:
    def __init__(self):
        self.x = 'x'
        self.o = 'o'
        self.empty = '.'
        self.data = [
            [self.empty for j in range(3)] for i in range(3)
        ]
        self.step = 1

    def info(self):
        #return '(%d) %s takes: ' % (self.step, self.turn())
        return '%s: ' % self.turn()

    def __str__(self):
        return '\n'.join(
            ' '.join(token for token in row)
            for row in self.data
        )

    def turn(self, step=None):
        if step == None:
            step = self.step
        return self.o if step & 1 else self.x # test mod 2

    def take(self, r, c):
        if r < 1 or r > 3 or c < 1 or c > 3:
            raise ValueError('index out of range')
        elif self.data[r-1][c-1] != self.empty:
            raise ValueError('position taken')

        self.data[r-1][c-1] = self.turn()
        self.step += 1

    def gameover(self, r, c):
        turn = self.turn(self.step-1)
        if all(self.data[r-1][i] == turn for i in range(3))\
                or all(self.data[i][c-1] == turn for i in range(3))\
                or all(self.data[i][i] == turn for i in range(3))\
                or all(self.data[i][2-i] == turn for i in range(3)):
            return 'Hooray, %s wins!' % turn
        elif self.step == 10:
            return 'Draw!'

# ---- class Board ends

def cursor_up(n):
    print('\033[%dA' % n, end='')

def cursor_down(n):
    print('\033[%dB' % n, end='')

def cursor_right(n):
    print('\033[%dC' % n, end='')

def cursor_left(n):
    print('\033[%dD' % n, end='')

def cursor_goto(l, c=0):
    print('\033[%d;%dH' % (l, c), end='')
    # or return '\033%d;%df' % (l, c)

def screen_clear():
    print('\033[2J', end='')
    cursor_goto(0, 0)

def line_clear(l=None):
    if l:
        cursor_goto(l)
    print('\033[K', end='')

def yes(prompt):
    s = input(prompt + ' (y/n) ')
    return len(s) != 0 and (s[0] == 'y' or s[0] == 'Y')

def run():
    b = Board()
    screen_clear()
    print(b)
    while True:
        try:
            line_clear(4)
            s = input(b.info())
            line_clear(5)
        except EOFError:
            if yes('\nquit game?'):
                exit()
            line_clear(5)
            continue

        try:
            r, c = map(int, s.split())
        except Exception:
            print('invalid input')
            continue

        try:
            b.take(r, c)
        except ValueError as e:
            print(e)
            continue

        cursor_goto(0)
        print(b)
        s = b.gameover(r, c)
        if s:
            line_clear(4)
            print(s)
            break

while True:
    run()
    if not yes('new game?'):
        break
    line_clear(5)
