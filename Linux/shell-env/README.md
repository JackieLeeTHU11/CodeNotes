## shell 环境

1) printenv - 打印部分或所有的环境变量
2) set - 设置 shell 选项
3) export —导出环境变量,让随后执行的程序知道。
4) alias - 创建命令别名

### 检查环境变量

我们既可以用 bash 的内部命令 set,或者是 printenv 程序来查看什么存储在环境当中。set命令可以显示 shell 和环境变量两者,而 printenv 只是显示环境变量。因为环境变量内容列表相当长,所以最好把每个命令的输出结果管道到 less 命令:

	[me@linuxbox ~]$ printenv | less
		显示环境变量
		
当使用没有带选项和参数的 set 命令时,shell 和环境变量二者都会显示,同时也会显示定义的 shell 函数。不同于 printenv 命令,set 命令的输出结果很礼貌地按照字母顺序排列:

	[me@linuxbox ~]$ set | less
		显示环境变量和shell
		
可以通过 echo 命令来查看一个变量的内容:
	[me@linuxbox ~]$ echo $HOME
	
如果 shell 环境中的一个成员既不可用 set 命令也不可用 printenv 命令显示,则这个变量是别名。输入不带参数的 alias 命令来查看它们:

	[me@linuxbox ~]$ alias
	
### 一些有趣的变量

	shell 环境中包含相当多的变量,虽然你的 shell 环境可能不同于这里展示的,但是你可能会看到以下变量在你的 shell 环境中.

	DISPLAY 如果你正在运行图形界面环境,那么这个变量就是你显示器的名字。通常,它是”:0”,意思是由 X 产生的第一个显示
器。
	EDITOR 文本编辑器的名字。
	SHELL shell 程序的名字。
	HOME 用户家目录。
	LANG 定义了字符集以及语言编码方式。
	OLD PWD 先前的工作目录。
	PAGER 页输出程序的名字。这经常设置为/usr/bin/less。
	PATH 由冒号分开的目录列表,当你输入可执行程序名后,会搜索这个目录列表。
	PS1 Prompt String 1. 这个定义了你的 shell 提示符的内容。随后我们可以看到,这个变量内容可以全面地定制。
	PWD 当前工作目录。
	TERM 终端类型名。类 Unix 的系统支持许多终端协议;这个变量设置你的终端仿真器所用的协议。
	TZ 指定你所在的时区。大多数类 Unix 的系统按照协调时间时(UTC) 来维护计算机内部的时钟,然后应用一个由这个变量指定的偏差来显示本地时间。
	USER 你的用户名
	
### 如何建立 shell 环境

#### 非登录 shell 会话会读取以下启动文件
	
	/etc/bash.bashrc 应用于所有用户的全局配置文件。
	~/.bashrc 用户私有的启动文件。可以用来扩展或重写全局配置脚本中的设置。
	
#### 激活我们的修改

我们对于文件.bashrc 的修改不会生效,直到我们关闭终端会话,再重新启动一个新的会话,因为.bashrc 文件只是在刚开始启动终端会话时读取。然而,我们可以强迫 bash 重新读取修改过的.bashrc 文件,使用下面的命令:
	
	[me@linuxbox ~]$ source .bashrc


