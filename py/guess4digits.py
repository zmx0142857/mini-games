#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""A 4-digit guess game from my old television."""

__author__ = 'Clarence Zhuo'

import random

def init():
    L = [i for i in range(10)]
    answer = ''
    for i in range(4):
        answer += str(L.pop(random.randint(0, 9-i)))
    #print('answer:', answer)
    return answer

def isvalid(guess):
    msg = 'input must be a non-repeat 4-digit number'
    if len(guess) != 4:
        print('invalid input 1:', msg)
        return False
    if any(not c.isdigit() for c in guess):
        print('invalid input 2:', msg)
        return False
    for i in range(4):
        if guess[i] in guess[:i]:
            print('invalid input 3:', msg)
            return False
    return True

def judge(gus, ans):
    zipped = zip(gus, ans)
    cntA = 0
    for pair in zipped:
        if pair[0] == pair[1]:
            cntA += 1
    cntB = len(set(gus) & set(ans)) - cntA
    return cntA, cntB

def play():
    answer = init()
    cnt = 1
    while True:
        guess = input('guess %d: ' % cnt)
        if not isvalid(guess):
            continue
        result = judge(guess, answer)
        if result == (4, 0):
            print('you win!')
            break
        else:
            print('%dA %dB' % result)
            cnt += 1
    
    if input('another game? ')[0].lower() != 'y':
        exit()
try:
    while True:
        play()
except EOFError:
    print()
