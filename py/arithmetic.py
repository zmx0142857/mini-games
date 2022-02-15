#!/data/data/com.termux/files/usr/bin/python3
# -*- coding: utf-8 -*-

from random import randint, choice
from time import time
from sys import argv

correct = 0
mistake = 0
mistakes = []
begin = time()
beginq = begin
a = argv[1] if len(argv) > 1 else 12
b = argv[2] if len(argv) > 2 else 99
speed = {}

def stat():
    total = correct + mistake
    print("\rcorrect: %d/%d" % (correct, total))
    if total > 0:
        print("speed: %f questions per minute" % (60*correct/(time()-begin)))
        print('\n'.join('%d: %f' % e for e in sorted([(k, 60*speed[k][1]/speed[k][0]) for k in speed], key=(lambda t:t[1]), reverse=True)))
        if mistakes:
            print("mistakes: %s" % ', '.join('%d*%d' % elem for elem in set(mistakes)))

def get_operands(a, b):
    if len(mistakes) > 0 and randint(0,6) == 0:
        return choice(mistakes)
    else:
        return randint(a,b), randint(2,9)

for cnt in range(30):
    i, j = get_operands(a, b)
    ans = i * j
    while True:
        try:
            user = int(input("%d x %d = " % (i, j)))
        except EOFError:
            stat()
            exit()
        except ValueError:
            print("invalid input")
            continue
        if user == ans:
            correct += 1
            now = time()
            if i in speed:
                speed[i] = (speed[i][0]+now-beginq, speed[i][1]+1)
            else:
                speed[i] = (now-beginq, 1)
            beginq = now
        else:
            mistake += 1
            print("answer: %d" % ans)
            mistakes.append((i,j))
        break
stat()
