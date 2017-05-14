# coding=utf-8
# 最多有多少种找零钱的方法

class Penny():
    def __init__(self):
        self.penny=[1,2,4]
        self.n=3
        self.aim=4
    def countWays(self):
        dp = [[0 for j in range(self.aim+1)] for i in range(self.n)]
        for j in range(0, self.aim+1):
            if j % self.penny[0] == 0:
                dp[0][j] = 1
        for i in range(1, self.n):
            dp[i][0]=1
            for j in range(1, self.aim+1):
                if j>=self.penny[i]:
                    dp[i][j] = dp[i-1][j] + dp[i][j-self.penny[i]]
                else:
                    dp[i][j] = dp[i-1][j]
        return dp
def countpenny(penny,n,aim):
	dp = [[0 for j in range(aim+1)] for i in range(n)]
	for j in range(aim+1):
		if j%penny[0] == 0:
			dp[0][j] = 1
	for i in range(1,n):
		dp[i][0] = 1
		for j in range(1,aim+1):
			dp[i][j] = dp[i-1][j]
			if penny[i]<=j:
				dp[i][j] = dp[i-1][j] + dp[i][j-penny[i]]
			else:
				dp[i][j] = dp[i-1][j]
				
	return dp


if __name__ == "__main__":
    penny = Penny()
    print penny.countWays()
    print "========================="
    penny=[1,2,4]
    n=3
    aim=10
    print countpenny(penny,n,aim)
    
    
