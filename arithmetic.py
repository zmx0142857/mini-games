#!/data/data/com.termux/files/usr/bin/python3
# -*- coding: utf-8 -*-

from random import randint, choice
from time import time
from sys import argv

correct = 0
mistake = 0
mistakes = []
begin = time()
a = 12
b = 99
if len(argv) > 2:
    a, b = int(argv[1]), int(argv[2])
while True:
    if len(mistakes) > 0 and randint(0,6) == 0:
        elem = choice(mistakes)
        i, j = elem['i'], elem['j']
    else:
        i, j = randint(a,b), randint(2,9)
    ans = i * j
    while True:
        try:
            user = int(input("%d x %d = " % (i, j)))
        except EOFError:
            total = correct + mistake
            print("\rcorrect: %d/%d" % (correct, total))
            if total > 0:
                print("speed: %f questions per minute" % (60*correct/(time()-begin)))
            exit()
        except ValueError:
            print("invalid input")
            continue
        if user == ans:
            correct += 1
        else:
            mistake += 1
            print("answer: %d" % ans)
            mistakes.append({'i':i, 'j':j})
        break
