import numpy as np


grid = 6
a = np.zeros((grid,grid))
walls = np.zeros((grid,grid))

walls = np.array([
	# [11,8,10,8,10,12],
	# [9,6,9,2,12,5],
	# [5,13,1,14,3,4],
	# [1,4,5,9,12,5],
	# [5,3,6,5,5,5],
	# [3,10,10,6,3,6]

	[0,0,0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,4,5,4],
	[0,0,0,4,3,4],

	])
print('walls')
print(walls)


n = 1			#goal value
a[5,0] = n 		#setting location of goal	

#print(a)

while n < 50:

	for i in range(0,grid):
		for j in range(0,grid):

			# print('n is ' +str(n))
			# print('i is ' + str(i))
			# print('j is ' + str(j)+'\n')
		
			if a[i,j] == n:
				#print('yes')
				if (i+1)<grid:
					#Check South side
					if a[i+1,j] != 1 and a[i+1,j] == 0 and (walls[i,j] & 2 == 0):
						#print('1')
						a[i+1,j] = n+1	
						#print(a)

				if (i-1)>-1:
					#check North
					if a[i-1,j] != 1 and a[i-1,j] == 0 and (walls[i,j] & 8 == 0):
						#print('2')
						a[i-1,j] = n+1
						#print(a)

				if(j+1)< grid:
					#check east
					#print('no')
					if a[i,j+1] != 1 and a[i,j+1] == 0 and (walls[i,j] & 4 == 0):
						#print('3')
						a[i,j+1] = n+1
						#print(a)

				if (j-1)>-1:
					#check weset
					if a[i,j-1] != 1 and a[i,j-1] == 0 and (walls[i,j] & 1 == 0):
						#print('4')
						a[i,j-1] = n+1
						#print(a)


	n+=1

	
print('final array')
print(a)
