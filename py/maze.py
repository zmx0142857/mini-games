"""A maze game generator"""

__author__ = 'zmx0142857'

import random

class Maze(object):

    def __init__(self, rows, cols):

        walls = ('┌','┬','┐','├','┼','┤','└','┴','┘','─','│')
        specials = ('A','B')

        def my_choice(i, j):
            top = (i == 0)
            bottom = (i == rows-1)
            left = (j == 0)
            right = (j == cols-1)
            if left:
                if top:
                    return '┌'
                elif bottom:
                    return '└'
                else:
                    return random.choice(('├','│'))
            elif right:
                if top:
                    return '┐'
                elif bottom:
                    return '┘'
                else:
                    return random.choice(('┤','│'))
            elif top:
                return random.choice(('┬','─'))
            elif bottom:
                return random.choice(('┴','─'))
            else:
                return random.choice(walls)

        self._map = '\n'.join(''.join(my_choice(i, j) for j in range(cols)) for i in range(rows))

    def __str__(self):
        return self._map

    __repr__ = __str__

    def detailize(self):
        pass
