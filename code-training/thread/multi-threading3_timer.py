# coding=utf-8

from threading import Timer

def hello():
    print "hello, world"
t = Timer(3, hello)
t.start() # 3秒钟之后执行hello函数。
