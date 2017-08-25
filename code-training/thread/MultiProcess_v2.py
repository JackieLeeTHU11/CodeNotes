# -*- coding: UTF-8 -*- 
# Authorized by Vlon Jang
# Created on 2017-06-13
# Blog: www.wangqingbaidu.cn
# Email: wangqingbaidu@gmail.com
# From kwai, www.kuaishou.com
# ©2015-2017 All Rights Reserved.
#

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import multiprocessing
import math

class MultiProcess():
    """
        MultiProcess, Give total samples to deal with, a function and kwargs and split parts.
        
        multiprocess.start()
        multiprocess.wait()
    """
    def __init__(self, samples, func, num_process=1, **kwargs):
        self.samples = samples
        self.func = func
        self.kwargs = kwargs
        self.num_process = num_process
        
        self.thread_list = []
        
    def start(self):
        if self.samples and self.func and self.num_process:
            samples_per_thread = int(math.ceil(len(self.samples) / self.num_process))
            
            for i in range(self.num_process):
                kwargs = {'thread_id':i, 
                          'samples':self.samples[samples_per_thread * i: samples_per_thread * (i + 1)]}
                kwargs.update(self.kwargs)
                sthread = multiprocessing.Process(target=self.func, kwargs=kwargs)
                print ('Starting process %d from %d to %d ...' %(i, 
                                                                samples_per_thread * i, 
                                                                min(len(self.samples), 
                                                                    samples_per_thread * (i + 1))))
                sthread.start()
                self.thread_list.append(sthread)
        else:
            print ('Error while starting multi process',
                   'Please check `samples`, `func`, `num_process` parameters')
    def wait(self):     
        for t in self.thread_list:
            t.join()
            
    def run(self):
        self.start()
        self.wait()