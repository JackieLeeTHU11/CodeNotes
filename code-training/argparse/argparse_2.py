# coding=utf-8
#!/usr/bin/python  

  
from argparse import ArgumentParser  
import sys
  
p = ArgumentParser(usage='it is usage tip', description='this is a test')  
p.add_argument('--one', default=1, type=int, help='the first argument')  
p.add_argument('--two', default=2, type=int, help='the second argument')  
p.add_argument('--docs-dir', default="./", help='document directory')  
  
args = p.parse_args("--one 1 --two 20 --docs-dir /media/jackie".split())  
  
#可以打印出来查看  
print args  
  
#打印某一个参数  
print args.one  
print args.docs_dir   #经过parse_args()函数后参数名称去掉了前面的"--"，所有的"-"转化为"_"

# =================================================================================================
print "*******************************************************"

# 这个函数将认识的和不认识的参数分开放进2个变量中  
args, remaining = p.parse_known_args(sys.argv)  
  
#可以打印出来查看  
print args  
print remaining  

# =================================================================================================
print "*******************************************************"

from argparse import ArgumentParser  
import sys
  
p = ArgumentParser(usage='it is usage tip', description='this is a test')  
p.add_argument('--one', default=1, type=int, help='the first argument')  
p.add_argument('--const-value', action='store_const', const=42)  
p.add_argument('--foo', action='store_true')
p.add_argument('--bar', action='store_false')
p.add_argument('--baz', action='store_false')
p.add_argument('--docs-dir', default="./", help='document directory')  

  
args = p.parse_args("--one 1  --docs-dir /media/jackie --foo --bar".split())  
  
#可以打印出来查看  
print args  
  
#打印某一个参数  
print args.one  
print args.docs_dir   #经过parse_args()函数后参数名称去掉了前面的"--"，所有的"-"转化为"_"
