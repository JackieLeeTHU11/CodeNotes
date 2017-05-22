## 进程

### 把一个进程放置到后台 (执行)
比方说,我们想让 shell 提示符返回,却没有终止 xlogo 程序。为达到这个目的,我们把这个程序放到后台执行。把终端看作是一个有前台(表层放置可见的事物,像 shell 提示符)和后台(表层之下放置隐藏的事物)(的设备)。启动一个程序,让它立即在后台运行,我们在程序命令之后,加上 “&” 字符:

	xlogo &

### 进程返回到前台

一个在后台运行的进程对一切来自键盘的输入都免疫,也不能用 Ctrl-c 来中断它。使用 fg 命令,让一个进程返回前台执行

	[me@linuxbox ~]$ jobs
		[1]+ Running xlogo &
	[me@linuxbox ~]$ fg %1
		xlogo

### 停止一个进程(并不是终止一个进程)

有时候,我们想要停止一个进程,而没有终止它。这样会把一个前台进程移到后台等待。输入 Ctrl-z,可以停止一个前台进程。

	使用 fg 命令,可以恢复程序到前台运行,或者用 bg 命令把程序移到后台。


### 通过 kill 命令给进程发送信号

kill 命令被用来给程序发送信号。它最常见的语法形式看起来像这样:
	kill [-signal] PID...
	
	
	
	
	
	
	
	
	
	
	
