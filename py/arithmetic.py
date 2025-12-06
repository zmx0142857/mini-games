from random import randint, choice
from time import time
from sys import argv

correct = 0
mistake = 0
mistakes = []
begin = time()
beginq = begin
if len(argv) < 3:
    print('usage: python arithmetic.py min max [count]\nexample: python arithmetic.py 12 15')
    exit(0)
a = int(argv[1])
b = int(argv[2])
count = int(argv[3]) if len(argv) >= 4 else 15
speed = {}

def stat():
    total = correct + mistake
    print("\rcorrect: %d/%d" % (correct, total))
    if total > 0:
        speed_value = 60*correct/(time()-begin)
        print(f"speed: {speed_value:.2f}")
        print('\n'.join(f'   {v[0]}: {v[1]:.2f}' for v in sorted([(k, 60*speed[k][1]/speed[k][0]) for k in speed], key=(lambda t:t[1]), reverse=True)))
        if mistakes:
            print("mistakes: %s" % ', '.join('%d*%d' % elem for elem in set(mistakes)))

def get_operands(a, b):
    if len(mistakes) > 0 and randint(0,6) == 0:
        return choice(mistakes)
    else:
        return randint(a,b), randint(2,9)

for cnt in range(count):
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
