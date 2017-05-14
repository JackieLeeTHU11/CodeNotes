# coding=utf-8
#---- Event
#---- 捉迷藏的游戏
import threading, time
class Hider(threading.Thread):
    def __init__(self, cond, name):
        super(Hider, self).__init__()
        self.cond = cond
        self.name = name
    
    def run(self):
        time.sleep(1) #确保先运行Seeker中的方法   
        
        print self.name + ': 我已经把眼睛蒙上了'
        
        self.cond.set()
        
        time.sleep(1)   
        
        self.cond.wait()
        print self.name + ': 我找到你了 ~_~'
        
        self.cond.set()
                            
        print self.name + ': 我赢了'
        
class Seeker(threading.Thread):
    def __init__(self, cond, name):
        super(Seeker, self).__init__()
        self.cond = cond
        self.name = name
    def run(self):
        self.cond.wait()
                        
        print self.name + ': 我已经藏好了，你快来找我吧'
        self.cond.set()
        
        time.sleep(1)
        self.cond.wait()
                            
        print self.name + ': 被你找到了，哎~~~'
        
cond = threading.Event()
seeker = Seeker(cond, 'seeker')
hider = Hider(cond, 'hider')
seeker.start()
hider.start()
