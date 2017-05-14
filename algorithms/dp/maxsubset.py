# coding=utf-8

# 两种方法找数组的子数组的最大和
# 找子数组的最大和
class Solution:
    def FindGreatestSumOfSubArray(self, array):
        if not array:
            return 0
        rest = []
        for i in range(len(array)):
            sum = array[i]
            rest.append(sum)
            for j in range(i+1,len(array)):
                sum += array[j]
                rest.append(sum)
        rest.sort()
        return rest[-1]
mylist = [-2,1,16,1, -2, 3, 10, -4, 7, 2, -5]
mylist = [-2,1,-3,4,-1,2,1,-5,4]
#solution = Solution()
#result = solution.FindGreatestSumOfSubArray(mylist)
#print result

# 动态规划的方法找最大和
def findmaxset(inputlist):
	nall = inputlist[0]	#当前最大子数组的和
	nend = 0	#以A[i]结尾的子数组最大和
	for i in range(len(inputlist)):
		nend = max(inputlist[i], nend+inputlist[i])
		nall = max(nall, nend)
	return nall

# print findmaxset(mylist)


def findmaxsubset(inputlist):
	# dp method
	sumn = inputlist[0]
	endn = 0
	for i in range(len(inputlist)):
		endn = max(endn+inputlist[i], inputlist[i]) # 以数据i为结尾的数组的最大和
		sumn = max(sumn, endn) #有数据i时,最大可能的和 
		print endn, sumn
	return sumn
	
print findmaxsubset(mylist)

