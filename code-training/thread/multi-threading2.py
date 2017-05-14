# coding:utf-8

import threading, time, random
count = 0
class Counter(threading.Thread):
    def __init__(self, lock, threadName):
        '''@summary: 初始化对象。
        
        @param lock: 琐对象。
        @param threadName: 线程名称。
        '''
        super(Counter, self).__init__(name = threadName)  #注意：一定要显式的调用父类的初始化函数。
        self.lock = lock
    
    def run(self):
        '''@summary: 重写父类run方法，在线程启动后执行该方法内的代码。
        '''
        global count
        self.lock.acquire()
        for i in xrange(10000):
            count = count + 1
        self.lock.release()
lock = threading.Lock()
for i in range(1): 
    Counter(lock, "thread-" + str(i)).start()
time.sleep(2)	#确保线程都执行完毕
print count

# ===================================================================

import threading, time, random
count = 0
lock = threading.Lock()

def doAdd():
    '''@summary: 将全局变量count 逐一的增加10000。
    '''
    global count, lock
    lock.acquire()
    for i in xrange(10000):
        count = count + 1
    lock.release()
    
for i in range(2):
    threading.Thread(target = doAdd, args = (), name = 'thread-' + str(i)).start()
    
time.sleep(2)	#确保线程都执行完毕
print count

# ===================================================================

import threading, time

def doWaiting():
    print 'start waiting:', time.strftime('%H:%M:%S')
    time.sleep(3)
    print 'stop waiting', time.strftime('%H:%M:%S')
    
thread1 = threading.Thread(target = doWaiting,name="thread1")
thread1.start()
time.sleep(1)  #确保线程thread1已经启动
print 'the %s %s at %s'%(thread1.getName(),"start", time.strftime('%H:%M:%S'))
thread1.join()	#将一直堵塞，直到thread1运行结束。
print 'the %s join at %s'%(thread1.getName(), time.strftime('%H:%M:%S'))



