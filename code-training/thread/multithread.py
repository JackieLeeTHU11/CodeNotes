# coding:utf-8

# multi-thread

import threading
from time import ctime, sleep

def music(song="康定情歌"):
	for i in range(3):
		print "I am listening to %s, time: %s"%(song, ctime())
		sleep(5)
		
def movie(video="黑影坠落"):
	for i in range(3):
		print "I am watching the %s, time: %s"%(video, ctime())
		sleep(1)
		
threads = []
thread1 = threading.Thread(target=music, args=("I really love you!",))
threads.append(thread1)
thread2 = threading.Thread(target=movie, args=("越狱5",))
threads.append(thread2)

if __name__=="__main__":
#	for t in threads:
#		t.setDaemon(True)
#		t.start()
	thread1.setDaemon(True)
	thread1.start()
	thread2.setDaemon(True)
	thread2.start()
	thread1.join()
	thread2.join()
	print "finished %s"%(ctime())
	
	
	
	
