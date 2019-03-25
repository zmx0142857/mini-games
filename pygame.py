class Console:
    def __init__(self, line_msg=None, line_input=None):
        self.line_msg = line_msg
        self.line_input = line_input

    def cursor_up(self, n):
        print('\033[%dA' % n, end='')

    def cursor_down(self, n):
        print('\033[%dB' % n, end='')

    def cursor_right(self, n):
        print('\033[%dC' % n, end='')

    def cursor_left(self, n):
        print('\033[%dD' % n, end='')

    def cursor_goto(self, l, c=0):
        print('\033[%d;%dH' % (l, c), end='')
        # or '\033%d;%df' % (l, c)

    def screen_clear(self):
        print('\033[2J', end='')
        self.cursor_goto(0, 0)

    def line_clear(self, l=None):
        if l != None:
            self.cursor_goto(l)
        print('\033[K', end='')

    def input(self, prompt):
        if self.line_input != None:
            self.line_clear(self.line_input)
        s = input(prompt)
        if self.line_msg != None:
            self.line_clear(self.line_msg)
        return s

    def msg(self, *s):
        if self.line_msg != None:
            self.line_clear(self.line_msg)
        print(*s)

    def yes(self, prompt):
        try:
            s = self.input(prompt + ' (y/n) ')
        except Exception:
            return False
        return len(s) != 0 and (s[0] == 'y' or s[0] == 'Y')
