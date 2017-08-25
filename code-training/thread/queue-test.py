#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2017/7/25 10:10
# @File    : que.py

#生产者消费者，阻塞队列实现



import random
from  Queue import  Queue
import  threading
import time


class Producer(threading.Thread):
    """
    @:param queue 阻塞队列
    @:param name 线程名字
    """
    def __init__(self,queue,name):
        threading.Thread.__init__(self)
        self.queue = queue
        self.name = name


    def run(self):
        while True:
            integer = random.randint(0,350)
            self.queue.put(integer)
            print self.name,'将',integer,'加入队列'
            time.sleep(6)



class Consumer(threading.Thread):
    def __init__(self,queue,name):
        threading.Thread.__init__(self)
        self.queue = queue
        self.name = name


    def run(self):
        while True:
            integer = self.queue.get()
            print self.name,'将',integer,'从队列中移除'
            self.queue.task_done()


def main():
    queue = Queue(32)
    consumer = Consumer(queue,'消费者')
    producer = Producer(queue,'生产者')
    producer.start()
    consumer.start()

    if __name__ == '__main__':
        main()

main()