#!/usr/bin/env python3

__author__ = '先知-6047-@bilibili'

# 自带
import sys, os, time, threading

# 需要另外安装. 其中 cv2 的安装方式是 pip3 install opencv-python
import cv2, pyprind

class CharFrame:
    #ascii_char = '@#B%WM8&$wmoahkbdpqZ0QOLCJUYXzcvunxrjft*/\|()1{}[]?-_+~<>i!lI;:,"^`\'. '
    ascii_char = ' .`\'^",:;Il!i<>~+_-?[]{}1()|\/*tjfrxnuvczXYUJCLOQ0Zqpbdkhaomw$&8MW%B#@'

    # 像素映射到字符
    def pixel2Char(self, luminance):
        return self.ascii_char[luminance * len(self.ascii_char) >> 8]

    # 将普通帧转为 ASCII 字符帧
    def convert(self, img, limitSize=-1, fill=False, wrap=False):
        if limitSize != -1 and (img.shape[0] > limitSize[1]
                or img.shape[1] > limitSize[0]):
            img = cv2.resize(img, limitSize, interpolation=cv2.INTER_AREA)
        blank = ''
        if fill:
            blank += ' ' * (limitSize[0] - img.shape[1])
        if wrap:
            blank += '\n'
        return blank.join(
            ''.join(
                self.pixel2Char(img[i,j]) for j in range(img.shape[1])
            ) for i in range(img.shape[0])
        )

class V2Char(CharFrame):
    charVideo = []
    timeInterval = 0.033

    def __init__(self, path):
        if path.endswith('.txt'):
            self.load(path)
        else:
            self.build(path)
            self.save(path.rsplit('.', 1)[0] + '.txt')

    def build(self, path):
        self.charVideo = []
        # 用 opencv 读取视频
        cap = cv2.VideoCapture(path)
        self.timeInterval = round(1 / cap.get(5), 3)
        nf = int(cap.get(7))
        print('Generating char video...')
        for i in pyprind.prog_bar(range(nf)):
            # 转换颜色空间
            rawFrame = cv2.cvtColor(cap.read()[1], cv2.COLOR_BGR2GRAY)
            frame = self.convert(rawFrame, os.get_terminal_size(), fill=True)
            self.charVideo.append(frame)
        cap.release()

    def save(self, path):
        if not self.charVideo:
            return
        with open(path, 'w') as f:
            f.write('\n'.join(self.charVideo) + '\n')

    def load(self, path):
        self.charVideo = [frame[:-1] for frame in open(path)]

    def play(self):
        if not self.charVideo:
            return
        interrupt = False
        def getChar():
            nonlocal interrupt
            try:
                # 若系统为 windows 则直接调用 msvcrt.getch()
                import msvcrt
                if msvcrt.getch():
                    interrupt = True
            except ImportError:
                import termios, tty
                fd = sys.stdin.fileno()              # 获取文件描述符
                old_settings = termios.tcgetattr(fd) # 保存旧设置
                tty.setraw(sys.stdin.fileno())       # 设置输入为原始模式
                ch = sys.stdin.read(1)               # 线程在这里阻塞
                # 恢复旧设置
                termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
                if ch:
                    interrupt = True

        # 用后台线程监听按键
        getchar = threading.Thread(target=getChar, daemon=True)
        getchar.start()

        for frame in self.charVideo:
            # 接收到输入则退出循环
            if interrupt:
                break
            print(frame, end='', flush=True)
            time.sleep(self.timeInterval)
            print('\033[0;0H', end='') # 光标回到 0,0
        rows = len(self.charVideo[0]) // os.get_terminal_size()[0]
        print('\033[%d;0H' % rows) # 光标移到最后一行
        if interrupt:
            print('KeyboardInterrupt')
        else:
            print('Press any key to exit', end='', flush=True)
            getchar.join()
            print('')

if __name__ == '__main__':
    if len(sys.argv) == 1:
        print('usage: %s <file>' % sys.argv[0])
    else:
        v2char = V2Char(sys.argv[1])
        v2char.play()
