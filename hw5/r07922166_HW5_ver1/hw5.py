import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

img = Image.open('lena.bmp').convert('L')
img_array = np.array(img)

kernel = np.ones(shape=(8,8))

def changeToBinary(img, threshold):
	for index1, row in enumerate(img):
		for index2, ele in enumerate(row):
			if ele < threshold:
				img[index1][index2] = 0
			else:
				img[index1][index2] = 1
	return img

def shrinkImage(img):

	result = np.zeros(shape=(int(img.shape[0]/kernel.shape[0]),int(img.shape[1]/kernel.shape[1])))
	for i in range(0, int(img.shape[0]/kernel.shape[0])):
		for j in range(0, int(img.shape[1]/kernel.shape[1])):
			result[i][j] = img[i*8][j*8]
	return result

def yokoiNumber(inputMat):
	# input : 3x3 matrix
	qCount = 0
	rCount = 0
	center = [(1,1),(1,0),(2,0),(2,1)]
	for rotate, c in enumerate(center):
		row, col = c
		b = inputMat[row][col]
		c = inputMat[row][col+1]
		d = inputMat[row-1][col+1]
		e = inputMat[row-1][col]
		for x in range(rotate):
			temp = b
			b = c
			c = d
			d = e
			e = temp

		if b == c == 1:
			if d == b and e == b:
				rCount += 1
			elif d != b or e != b:
				qCount += 1
	if rCount == 4:
		return 5
	else:
		return qCount


temp = changeToBinary(img_array, 128)
plt.imshow(temp, cmap='gray')
plt.show()
shrinked = shrinkImage(temp)
plt.imshow(shrinked, cmap='gray')
plt.show()
result = np.zeros(shape=shrinked.shape)
for index1, row in enumerate(shrinked):
	for index2, ele in enumerate(row):
		if ele == 1:
			inputMat = np.zeros(shape=(3,3))
			x = index1 > 0 
			y = index2 > 0
			index1 *= x
			index2 *= y
			if x:
				inputMat[0][0] = shrinked[index1-1][index2-1]
				inputMat[0][1] = shrinked[index1-1][index2]
	
				try:
					inputMat[0][2] = shrinked[index1-1][index2+1]
				except:
					pass

			if y:
				inputMat[1][0] = shrinked[index1][index2-1]

			inputMat[1][1] = shrinked[index1][index2]
			
			try:
				inputMat[1][2] = shrinked[index1][index2+1]
			except:
				pass
			try:
				inputMat[2][0] = shrinked[index1+1][index2-1]
			except:
				pass
			try:
				inputMat[2][1] = shrinked[index1+1][index2]
			except:
				pass
			try:
				inputMat[2][2] = shrinked[index1+1][index2+1]
			except:
				pass
			result[index1][index2] = yokoiNumber(inputMat)
			
			
			
plt.imshow(result, cmap='gray')
plt.show()


for row in result:
	print('')
	for ele in row:
		if ele!=0:
			print(int(ele), end='')
			# print(' ',end='')
		else:
			print(' ', end='')



