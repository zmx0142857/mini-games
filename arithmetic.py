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
a = 12
b = 99
speed = {}

def stat():
    total = correct + mistake
    print("\rcorrect: %d/%d" % (correct, total))
    if total > 0:
        print("speed: %f questions per minute" % (60*correct/(time()-begin)))
        print('\n'.join('%d: %f' % e for e in sorted([(k, 60*speed[k][1]/speed[k][0]) for k in speed], key=(lambda t:t[1]), reverse=True)))
        if mistakes:
            print("mistakes: %s" % ', '.join('%d*%d' % elem for elem in set(mistakes)))

if len(argv) > 2:
    a, b = int(argv[1]), int(argv[2])
for cnt in range(30):
    if len(mistakes) > 0 and randint(0,6) == 0:
        i, j = choice(mistakes)
    else:
        i, j = randint(a,b), randint(2,9)
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
