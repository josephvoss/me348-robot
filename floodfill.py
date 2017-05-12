import numpy as np


grid = 6
a = np.zeros((grid,grid))


n = 1			#goal value
a[2,3] = n 		#setting location of goal	

#print(a)

while n < 12:

	for i in range(0,grid):
		for j in range(0,grid):

			print('n is ' +str(n))
			print('i is ' + str(i))
			print('j is ' + str(j)+'\n')
		
			if a[i,j] == n:
				print('yes')
				if (i+1)<grid:
					#Check South side
					if a[i+1,j] != 1 and a[i+1,j] == 0:
						print('1')
						a[i+1,j] = n+1	
						print(a)

				if (i-1)>-1:
					#check North
					if a[i-1,j] != 1 and a[i-1,j] == 0:
						print('2')
						a[i-1,j] = n+1
						print(a)

				if(j+1)< grid:
					#check east
					print('no')
					if a[i,j+1] != 1 and a[i,j+1] == 0:
						print('3')
						a[i,j+1] = n+1
						print(a)

				if (j-1)>-1:
					#check weset
					if a[i,j-1] != 1 and a[i,j-1] == 0:
						print('4')
						a[i,j-1] = n+1
						print(a)


	n+=1

	
print('final array')
print(a)
