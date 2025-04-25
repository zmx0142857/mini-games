from itertools import product, permutations

add = lambda x, y: x + y
sub = lambda x, y: x - y
mul = lambda x, y: x * y
div = lambda x, y: float('nan') if y == 0 else x / y
ops = [(add, '+'), (sub, '-'), (mul, '*'), (div, '/')]
nums = [1., 3., 4., 6.]

class Node:
    def __init__(self, op, left, right):
        self.op = op
        self.left = left
        self.right = right

    def __str__(self):
        return '(%s %s %s)' % (str(self.left), self.op[1], str(self.right))

    def __float__(self):
        return self.op[0](float(self.left), float(self.right))

    def test(self):
        epsi = 1e-6
        if abs(float(self) - 24) < epsi:
            print(str(self))

for a, b, c, d in permutations(nums):
    for f, g, h in product(ops, repeat=3):
        Node(f, Node(g, a, b), Node(h, c, d)).test()
        Node(f, Node(g, Node(h, a, b), c), d).test()
        Node(f, Node(g, a, Node(h, b, c)), d).test()
        Node(f, a, Node(g, b, Node(h, c, d))).test()
        Node(f, a, Node(g, Node(h, b, c), d)).test()
