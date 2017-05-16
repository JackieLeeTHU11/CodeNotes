## shell

### echo 
	echo this is a test. : 显示一行文本
	echo * : 显示路径下所有文档
	echo *D : 显示以D开头的文件名
	
	echo $((2 + 2)) : 算术表达式展开
		output:4
		注: 花括号展开模式可能包含一个开头部分叫做报头,一个结尾部分叫做附言。花括号表达式本身可能包含一个由逗号分开的字符串列表,或者一系列整数,或者单个的字符串。这种模式不能嵌入空白字符。
	echo Front-{A,B,C}-Back: 花括号展开
		output:Front-A-Back Front-B-Back Front-C-Back
	echo Number_{1..5}
		output:Number_1 Number_2 Number_3 Number_4 Number_5
		
		注: 那么这对什么有好处呢?最普遍的应用是,创建一系列的文件或目录列表。例如,如果我们是摄影师,有大量的相片。我们想把这些相片按年月先后组织起来。首先,我们要创建一系列以数值 “年-月” 形式命名的目录。通过这种方式,目录名按照年代顺序排列。我们可以键入整个目录列表,但是工作量太大了,并且易于出错。反而,我们可以这样做:

			mkdir {2007..2009}-0{1..9} {2007..2009}-{10..12}
	
	
	echo $USER : 参数展开
	printenv | less: 查看有效的变量列表
	
	
	echo $(ls) : 命令替换, 注:在linux下测试了, 应该是echo (ls)就可以替换了

### 双引号

如果你把文本放在双引号中,shell 使用的特殊字符,除了 $,\ (反斜杠),和 ‘(倒引号)之外,则失去它们的特殊含义,被当作普通字符来看待。这意味着单词分割,路径名展开,波浪线展开,和花括号展开都被禁止,然而参数展开,算术展开,和命令替换仍然执行。
	ls -l "two words.txt"

### 单引号

echo text ~/*.txt {a,b} $(echo foo) $((2+2)) $USER

	output: text /home/me/ls-output.txt a b foo 4 me

echo "text ~/*.txt {a,b} $(echo foo) $((2+2)) $USER"

	text ~/*.txt {a,b} foo 4 me
		注: 双引号禁止除了$,\ (反斜杠),和 ‘(倒引号)之外的转义,失去它们的特殊含义,被当作普通字符来看待。
	
echo 'text ~/*.txt {a,b} $(echo foo) $((2+2)) $USER'

	text ~/*.txt {a,b} $(echo foo) $((2+2)) $USER
		注: 单引号禁止所有转义
		
### 转义字符 \

有时候我们只想引用单个字符。我们可以在字符之前加上一个反斜杠,在这个上下文中叫做转义字符。

	echo "The balance for user $USER is: \$5.00"
		注: The balance for user me is: $5.00
	注:注意在单引号中,反斜杠失去它的特殊含义,它被看作普通字符。

反斜杠除了作为转义字符外,反斜杠也是一种表示法的一部分,这种表示法代表某种特殊字符,叫做控制码,常用的控制码:
	
	\a 响铃(“警告” -导致计算机嘟嘟响)
	\b 退格符
	\n 新的一行。在类 Unix 系统中,产生换行。
	\r 回车符
	\t 制表符
	
	实例: (使用 sleep 命令,一个简单的程序,它会等待指定的秒数,计算机声音提醒,然后退出。)
		sleep 10; echo -e ”Time’s up\a”
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
			




















