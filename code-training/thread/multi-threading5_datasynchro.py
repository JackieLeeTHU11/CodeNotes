# coding=utf-8
# thread A 和thread B 抢占CPU资源
# Python 在thread的基础上还提供了一个高级的线程控制库，就是之前提到过的threading。Python的threading module是在建立在thread module基础之上的一个module，在threading module中，暴露了许多thread module中的属性。在thread module中，python提供了用户级的线程同步工具“Lock”对象。而在threading module中，python又提供了Lock对象的变种: RLock对象。RLock对象内部维护着一个Lock对象，它是一种可重入的对象。对于Lock对象而言，如果一个线程连续两次进行acquire操作，那么由于第一次acquire之后没有release，第二次acquire将挂起线程。这会导致Lock对象永远不会release，使得线程死锁。RLock对象允许一个线程多次对其进行acquire操作，因为在其内部通过一个counter变量维护着线程acquire的次数。而且每一次的acquire操作必须有一个release操作与之对应，在所有的release操作完成之后，别的线程才能申请该RLock对象。

import threading  
mylock = threading.RLock()  
num=0  
   
class myThread(threading.Thread):  
    def __init__(self, name):  
        threading.Thread.__init__(self)  
        self.t_name = name  
          
    def run(self):  
        global num  
        while True:  
            mylock.acquire()  
            print 'Thread(%s) locked, Number: %d'%(self.t_name, num)
            if num>=4:  
                mylock.release()  
                print 'Thread(%s) released, Number: %d'%(self.t_name, num)
                break  
            num+=1  
            print 'Thread(%s) released, Number: %d'%(self.t_name, num)
            mylock.release()  
              
def test():  
    thread1 = myThread('A')  
    thread2 = myThread('B')  
    thread1.start()  
    thread2.start()  
   
if __name__== '__main__':  
    test()  
