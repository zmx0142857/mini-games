from itertools import groupby

def letterMap(word):
    return dict(map(lambda t: (t[0], len(list(t[1]))), groupby(word)))

def red(ch):
    return '\033[31m' + ch + '\033[0m'

def green(ch):
    return '\033[32m' + ch + '\033[0m'

def diff(word, guess):
    buf = []
    d = letterMap(word)
    for c1, c2 in zip(word, guess):
        if c1 == c2:
            buf.append(green(c2))
        else:
            cnt = d.get(c2, 0)
            if cnt:
                buf.append(red(c2))
                d[c2] = cnt-1
            else:
                buf.append(c2)
    return ''.join(buf)

def play(word):
    while True:
        try:
            guess = input('> ')
            if len(guess) != len(word):
                print('invalid word')
                continue
            print(diff(word, guess))
            if word == guess:
                break
        except EOFError:
            break

play('crane')
