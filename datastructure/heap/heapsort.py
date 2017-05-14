# coding=utf-8

# 堆排序
def heapsort(mylist):
	n = len(mylist)
	# 最大非叶子节点
	first = int(n/2-1)
	# 构造最大根堆
	for start in range(first, -1, -1):
		maxheapify(mylist, start, n-1)
		print mylist
	print "ok"
	# 把最大根堆转化为有序数组
	for end in range(n-1,0,-1):
		mylist[end], mylist[0] = mylist[0],mylist[end]
		maxheapify(mylist,0,end-1)
		print mylist
	return mylist
	
# 最大根调整,指定start节点转化为最大根节点,end为调整的下界
def maxheapify(mylist, start, end):
	root = start
	while True:
		child = root*2 +1 # child指定为root的left child
		if child>end: break # 如果出界,说明调整结束
		if child+1<end and mylist[child]<mylist[child+1]: # 如果rchild没有出界,并且lchild小于rchild, root和rchild比较
			child = child +1
		if mylist[root] < mylist[child]: # 如果root小于最大的孩子,则root和最大的孩子对掉
			mylist[root],mylist[child]=mylist[child],mylist[root]
			root=child
		else: # 反之,如果root大于最大的孩子,则本轮调整结束
			break
input_x = [2,6,4,6,23,2,56,73,32]
maxheapify(input_x, 0, len(input_x)-1)
print input_x
sortresult = heapsort(input_x)
print sortresult

