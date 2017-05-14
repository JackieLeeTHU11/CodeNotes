# coding=utf-8

class Node(object):
    def __init__(self, elem=-1, lchild = None, rchild=None):
        self.elem = elem
        self.lchild = lchild
        self.rchild = rchild

class Tree(object):
    def __init__(self):
        self.root = Node()
        self.myQuene = []

    def addnode(self, elem):
        node = Node(elem)

        if self.root.elem == -1:
            self.root = node
            self.myQuene.append(self.root)

        else:
            treenode = self.myQuene[0]
            if treenode.lchild==None:
                treenode.lchild=node
                self.myQuene.append(treenode.lchild)
            else:
                treenode.rchild=node
                self.myQuene.append(treenode.rchild)

                self.myQuene.pop(0)

def visit_front(root):
    if root==None:
        return
    print root.elem,
    visit_front(root.lchild)
    visit_front(root.rchild)

def visit_middle(root):
    if root==None:
        return
    # visit the left child first
    visit_middle(root.lchild)
    # visit the node self second
    print root.elem,
    # visit the right child third
    visit_middle(root.rchild)

def treedepth(root):
    if root==None:
        return 0
    leftdepth = treedepth(root.lchild)
    rightdepth = treedepth(root.rchild)
    return max(leftdepth,rightdepth)+1
    # return (leftdepth+1 if leftdepth>rightdepth else rightdepth+1)

def isbalance(root):
    if root==None:
        depth = 0
        return True, depth
    bleft, nLeftDepth = isbalance(root.lchild)
    bright, nRightDepth = isbalance(root.rchild)
    print "sub-tree depth (left child and right child):", bleft, nLeftDepth, bright, nRightDepth
    depth = 1 + (nLeftDepth if nLeftDepth > nRightDepth else nRightDepth)
    if (bleft&bright):
        diff = nRightDepth - nLeftDepth
        if ((diff<=1)&(diff>=-1)):
            return True, depth
        else:
            return False, depth
    else:
        return False, depth

def level_queue(root):
    """利用队列实现树的层次遍历"""
    if root == None:
        return
    myQueue = []
    node = root
    myQueue.append(node)
    while myQueue:
        node = myQueue.pop(0)
        print node.elem,
        if node.lchild != None:
            myQueue.append(node.lchild)
        if node.rchild != None:
            myQueue.append(node.rchild)
    print myQueue


# judge the balanced

def isbalancetree(root):
	if root==None:
		depth = 0
		return True, depth
	BTleft, depthleft = isbalancetree(root.lchild)
	BTright, depthright = isbalancetree(root.rchild)
	depth = 1+max(depthleft,depthright)
	if BTleft and BTright:
		diff = depthleft-depthright
		if (diff>=-1) and (diff<=1):
			return True, depth
		else:
			return False, depth
	else:
		return False, depth
		





if __name__=='__main__':

    tree = Tree()
    elems = range(16)
    for elem in elems:
        tree.addnode(elem)
    node = Node(100)
    tree.root.lchild.lchild.lchild.lchild.lchild = node
    print "front visit"
    visit_front(tree.root)
    print "finished"

    print "middle visit"
    visit_middle(tree.root)
    print "finished"

    print "tree depth:", treedepth(tree.root)

    print "whither tree is balanced:", isbalance(tree.root)
    level_queue(tree.root)
    
    print isbalancetree(tree.root)


