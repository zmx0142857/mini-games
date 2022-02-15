#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Class simulating the Chinese toy Jiulianhuan."""

__author__ = 'zmx0142857'

class Jiulianhuan(object):
    def __init__(self, loaded=True):
        self.reset(loaded)

    def reset(self, loaded=True):
        self.log = []
        # use 9-bit int to represent the status of jiulianhuan
        if loaded == True:
            self.status = (1 << 9) - 1
        elif loaded == False:
            self.status = 0
        elif isinstance(loaded, int):
            self.status = loaded
        else:
            raise TypeError("cannot set Jiulianhuan with type '%s'" 
                    % str(type(loaded)))

    def load(self, n):
        # load the first n rings. 1 <= n <= 9
        if n == 1:
            self.move(1)
            return
        if n == 2:
            self.move(1)
            self.move(2)
            return
        self.load(n-1)
        self.unload(n-2)
        self.move(n)
        self.load(n-2)

    def unload(self, n):
        # unload the first n rings. 1 <= n <= 9
        if n == 1:
            self.move(1)
            return
        if n == 2:
            self.move(2)
            self.move(1)
            return
        self.unload(n-2)
        self.move(n)
        self.load(n-2)
        self.unload(n-1)

    def move(self, n):
        # change the status, then log n. 1 <= n <= 9
        mask = 1 << (n-1)
        self.status ^= mask
        self.log.append(n if self.status & mask != 0 else -n)

    def view(self):
        # a straightforward view of current status
        return format(self.status, '09b')

j = Jiulianhuan()
j.unload(9)
print(j.log)
print('total step:', len(j.log))
