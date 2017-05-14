# coding=utf-8

class huffmanObject():
	def __init__(self,symbol=None, probability=None,codeWord='',proCode=''):
		self.symbol = symbol
		self.probability = probability
		self.codeWord = codeWord
		self.proCode = proCode
		self.lChild = None
		self.rChild = None
		self.father = None
		
class huffmanTree():
	def __init__(self,source):
		self.quene = source
	
	def createHuffmanTree(self):
		myqueue = self.quene[:]
		newqueue = []
		while (len(myqueue)>1):
			myqueue.sort(key=lambda item:item.probability)
			node_right = myqueue.pop(0)
			node_left = myqueue.pop(0)
			if node_left.symbol is not None:
				newqueue.append(node_left)
			if node_right.symbol is not None:
				newqueue.append(node_right)
			node_right.codeWord = '1'
			node_left.codeWord = '0'
			
			node_father = huffmanObject(probability=node_left.probability+node_right.probability)
			node_father.lChild = node_left
			node_father.rChild = node_right
			node_father.symbol = None
			node_left.father = node_father
			node_right.father = node_father
			myqueue.append(node_father) # add father in the quene
			
		myqueue[0].father = None # root has not father
		
		return newqueue, myqueue[0]

		
	def huffmanTreeEcoding(self, newqueue, root):
		probability = []  
		codes = ['']*len(newqueue)  
		symbol = []
		        
		for i in range(len(newqueue)):
			tmp_node = newqueue[i]
			while tmp_node.father != None:
				codes[i] = codes[i] + tmp_node.codeWord
				tmp_node = tmp_node.father
			
			newqueue[i].codeWord = codes[i]
			symbol.append(newqueue[i].symbol)
			probability.append(newqueue[i].probability)
			print "ok"
		
		return probability, codes, symbol
        
		
			
			
def printTree(root):
	if root==None:
		print "huffman tree has no object"
		return
	print root.symbol, root.codeWord
	printTree(root.lChild)
	printTree(root.rChild)

if __name__ == '__main__':  
    x1 = huffmanObject(symbol = "X1", probability = 0.375,codeWord = '' )    
    x2 = huffmanObject(symbol = "X2", probability = 0.125,codeWord = '' )      
    x3 = huffmanObject(symbol = "X3", probability = 0.25,codeWord = '')    
    x4 = huffmanObject(symbol = "X4", probability = 0.25,codeWord = '')       
    x5 = huffmanObject(symbol = "X5", probability = 0.125,codeWord = '')    
      
    tree = huffmanTree([ x1,x2, x3, x4, x5]) 
    newqueue, treeroot = tree.createHuffmanTree()
    a,b,c = tree.huffmanTreeEcoding([ x1,x2, x3, x4, x5], treeroot)
    print a,b,c
    printTree(treeroot)
		
