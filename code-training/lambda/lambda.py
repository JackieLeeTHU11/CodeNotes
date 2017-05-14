# coding=utf-8
# lambda 用法

# 定义函数（lambda表达式）
my_lambda = lambda arg : arg + 1
# 执行函数
result = my_lambda(123)
print result

# ====================================================
# map
# 遍历序列，对序列中每个元素进行操作，最终获取新的序列。

li = [11, 22, 33]
new_list = map(lambda a: a + 100, li)
print new_list

li = [11, 22, 33]
sl = [1, 2, 3]
new_list = map(lambda a, b: a + b, li, sl)
print new_list

# ====================================================
# filter
# 对于序列中的元素进行筛选，最终获取符合条件的序列
li = [11, 22, 33]
new_list = filter(lambda arg: arg > 22, li)
#filter第一个参数为空，将获取原来序列
print new_list

# ====================================================
# reduce
# 对于序列内所有元素进行累计操作

li = [11, 22, 33]

result = reduce(lambda arg1, arg2: arg1 + arg2, li)

# reduce的第一个参数，函数必须要有两个参数
# reduce的第二个参数，要循环的序列
# reduce的第三个参数，初始值
print result
