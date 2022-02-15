#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""tic tac toe"""

from game import Console
from time import sleep
from random import choice
import sys

class SortedSet(list):
    def __in__(self, elem):
        return self.bsearch(elem)[0]

    def append(self, elem):
        # do nothing if elem in self
        if elem not in self:
            list.append(self, elem)
            i = len(self)-2
            while i >= 0 and self[i] > elem:
                self[i+1] = self[i]
                i -= 1
            self[i+1] = elem

    def remove(self, elem):
        # do nothing if elem not in self
        isin, i = self.bsearch(elem)
        size = len(self)
        if isin:
            while i < size-1:
                self[i] = self[i+1]
                i += 1
            self.pop()

    def bsearch(self, elem):
        size = len(self)
        if size == 0:
            return False
        lo = 0
        hi = size-1
        while lo < hi:
            mid = (lo+hi) >> 1
            if elem > self[mid]:
                lo = mid+1
            elif elem < self[mid]:
                hi = mid-1
            else:
                return True, mid
        return elem == self[lo], lo

    def two_sum(self, elem):
        i = 0
        j = len(self)-1
        while i < j:
            if self[i] + self[j] < elem:
                i += 1
            elif self[i] + self[j] > elem:
                j -= 1
            else:
                return True, elem, i, j
        return False

# ---- class SortedSet ends

class Board:
    def __init__(self, level):
        self.x = 'x'
        self.o = 'o'
        self.empty = '.'
        self.value = [
            [4, 9, 2],
            [3, 5, 7],
            [8, 1, 6]
        ]
        self.pos = [ None,
            (2, 1), (0, 2), (1, 0),
            (0, 0), (1, 1), (2, 2),
            (1, 2), (2, 0), (0, 1)
        ]
        self.board = [
            [self.empty for j in range(3)] for i in range(3)
        ]
        self.step = 1
        self.available = SortedSet(i for i in range(1, 10))
        self.taken = [SortedSet(), SortedSet()]
        self.last = None
        self.gameover = None
        if level == 0:
            self.ai = self.ai0
        elif level == 2:
            self.ai = self.ai2
        else:
            self.ai = self.ai1

    def __str__(self):
        return '\n'.join(
            ' '.join(token for token in line) for line in self.board)

    def token(self, step=None):
        if step == None:
            step = self.step
        return self.o if step & 1 else self.x # test mod 2

    def info(self):
        return '%s: ' % self.token()

    def take(self, n):
        if n not in self.available:
            raise ValueError('position taken')

        if self.last != None:
            i, j = self.pos[self.last]
            self.board[i][j] = self.board[i][j][4]
        i, j = self.pos[n]
        self.board[i][j] = '\033[7m%s\033[0m' % self.token()
        self.last = n
        self.gameover = self.is_gameover()
        self.taken[self.step % 2].append(self.last)
        self.available.remove(self.last)
        self.step += 1

    def is_gameover(self):
        if self.taken[self.step % 2].two_sum(15-self.last):
            return '%s wins!' % self.token(self.step)
        elif self.step == 9:
            return 'draw!'
        return None

    def ai0(self):
        ret = choice(self.available)
        self.take(ret)
        return ret

    def ai1(self):
        # make his
        for n in self.available:
            if self.taken[self.step % 2].two_sum(15-n):
                self.take(n)
                return n
        # stuck yours
        for n in self.available:
            if self.taken[(self.step-1) % 2].two_sum(15-n):
                self.take(n)
                return n
        return self.ai0()

    def ai2(self):
        if 5 in self.available:
            self.take(5)
            return 5
        if self.step == 2:
            n = choice([2, 4, 6, 8])
            self.take(n)
            return n
        if self.step == 3 and self.taken[(self.step-1) % 2][0] & 1:
            n = choice([i for i in self.available if i % 2 == 0])
            self.take(n)
            return n
        return self.ai1()

# ---- class Board ends

class Game:
    def __init__(self, mode=1, level=1):
        try:
            self.mode = int(mode)
            self.level = int(level)
            if self.mode < 0 or self.mode > 3:
                raise ValueError
            if self.level < 0 or self.level > 2:
                raise ValueError
        except Exception:
            print('invalid arguments.\n'
                  'usage: tictactoe.py <mode> <level>\n'
                  'mode = 0 (pvp), 1 (pvc), 2 (cvp), 3 (cvc)\n'
                  'level = 0, 1, 2')
            exit(1)
        self.console = Console(line_msg=4, line_input=5)
        self.board = Board(level)

    def player_move(self):
        try:
            s = self.console.input(self.board.info())
        except EOFError:
            if self.console.yes('quit game?'):
                exit()
            return False
        try:
            line, col = map(int, s.split())
        except ValueError:
            self.console.msg('invalid input')
            return False
        try:
            n = self.board.value[line-1][col-1]
        except IndexError:
            self.console.msg('index out of range')
            return False
        try:
            self.board.take(n)
        except ValueError as e:
            self.console.msg(e)
            return False
        return True

    def computer_move(self):
        n = self.board.ai()
        line, col = self.board.pos[n]
        sleep(0.2)
        self.console.msg("computer takes %d %d" % (line+1, col+1))

    def gameover(self):
        self.console.cursor_goto(0)
        print(self.board)
        if self.board.gameover:
            self.console.msg(self.board.gameover)
            return True
        return False

    def pvp(self):
        while True:
            if not self.player_move():
                continue
            if self.gameover():
                break

    def pvc(self):
        while True:
            if not self.player_move():
                continue
            if self.gameover():
                break
            self.computer_move()
            if self.gameover():
                break

    def cvp(self):
        while True:
            self.computer_move()
            if self.gameover():
                break
            while not self.player_move():
                pass
            if self.gameover():
                break

    def cvc(self):
        while True:
            self.computer_move()
            if self.gameover():
                break

    def play(self):
        while True:
            self.board = Board(self.level)
            self.console.screen_clear()
            print(self.board)
            if self.mode == 0:
                self.pvp()
            elif self.mode == 1:
                self.pvc()
            elif self.mode == 2:
                self.cvp()
            elif self.mode == 3:
                self.cvc()
            if not self.console.yes('new game? '):
                break
    
if __name__ == '__main__':
    game = Game(*sys.argv[1:])
    game.play()
