#!/usr/bin/env python3
#-*- coding: utf-8 -*-

# Name:         Heart
# Author:       笑虎
# Version:      ??
# Link:         https://zhuanlan.zhihu.com/p/23321351
# Copyright:    著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处
# Description:  一行代码输出特定字符"Love"拼成的心形

print('\n'.join([''.join([('Love'[(x-y) % len('Love')] if ((x*0.05)**2+(y*0.1)**2-1)**3-(x*0.05)**2*(y*0.1)**3 <= 0 else ' ') for x in range(-30, 30)]) for y in range(14, -12, -1)]))
