#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""24 points"""

from random import randint

def welcome():
    print("""[24 points]

Enter a arithmetic expression using the 4 given numbers, try to make
result 24!  Note that you have only one chance for each problem.

    g - give up
    s - view score
    q - quit
""")

def parse(s, L):
    b = 0
    while b != len(s):
        while b != len(s) and not s[b].isdigit():
            b += 1
        e = b
        while e != len(s) and s[e].isdigit():
            e += 1
        if b == e:
            break
        try:
            i = int(s[b:e])
            idx = L.index(i)
            L.pop(idx)
        except Exception:
            return False
        b = e
    if len(L) != 0:
        return False
    try:
        val = eval(s)
    except Exception:
        return False
    if val != 24:
        return False
    if '//' in s:
        print('floor division is not allowed')
        return False
    if '#' in s:
        print('you thought this works?')
        return False
    for c in s:
        if c.isalpha():
            print('haha, I thought ahead of you!')
            return False
    return True

def run():
    score = 0
    welcome()
    while True:
        #L = [3, 7, 10, 12]
        L = [randint(1, 13) for i in range(4)]
        print(L)
        while True:
            s = input('> ')
            if s == 's':
                print('score', score)
            elif s == 'g': # give up
                print('you gave up')
                score -= 1
                break
            elif s == 'q': # quit
                print('game over\nscore:', score)
                return
            elif parse(s, L):
                print('yay!')
                score += 5
                break
            else:
                print('oops!')
                score -= 3
                break

run()
