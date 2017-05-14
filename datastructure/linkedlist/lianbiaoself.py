# coding=utf-8

class Node(object):
	def __init__(self, data, nextnode=None):
		self.data = data
		self.nextnode = nextnode
		
class LinkList(object):
	def __init__(self):
		self.head = None
	
	def initlinkerlist(self, inputlist):
		if len(inputlist) == 0:
			return
		for i in range(len(inputlist)):
			if i ==0:
				self.head = inputlist[0]
				p = self.head
			else:
				p.nextnode = inputlist[i]
				p = p.nextnode

			
	def getlength(self):
		if self.head == None:
			return 0
		length = 1
		p = self.head
		while p.nextnode != None:
			length += 1
			p = p.nextnode
		return length
		
	def isempty(self):
		if self.getlength() == 0:
			return True
		else:
			return False
	def printliker(self):
		if self.head==None:
			print "The linker is empty!"
			return
		p = self.head
		while p != None:
			print p.data,
			p = p.nextnode
		print "\nprint finished!"
	def insertnode(self, index ,node):
		if index<0 or index>self.getlength:
			print "index error"
		if index == 0:
			p = self.head
			q = node
			self.head = q
			q.nextnode = p
		else:
			num = 0
			p = self.head
			while num<index:
				p = p.nextnode
			if num==index and num!=(self.getlength-1):
				q = p.nextnode
				p.nextnode = node
				node.nextnode = q
			else:
				p.nextnode = node
			
			

# ================test================

node1 = Node(data=10)
node2 = Node(data=1)
node3 = Node(data=120)
node4 = Node(data=210)
node5 = Node(data=3110)

nodelist = [node1,node2,node3,node4,node5]

linker = LinkList()
linker.initlinkerlist(nodelist)
print linker.getlength()
linker.printliker()


	
	
