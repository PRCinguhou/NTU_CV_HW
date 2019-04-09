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


def getMatrix(enumerateMat, index1, index2, padding='zeros'):
	if padding == 'ones':
		inputMat = np.ones(shape=(3,3))
	else:
		inputMat = np.zeros(shape=(3,3))
	
	x = index1 > 0 
	y = index2 > 0
	index1 *= x
	index2 *= y
	if x:
		if y:
			inputMat[0][0] = enumerateMat[index1-1][index2-1]
		inputMat[0][1] = enumerateMat[index1-1][index2]
		try:
			inputMat[0][2] = enumerateMat[index1-1][index2+1]
		except:
			pass

	if y:
		inputMat[1][0] = enumerateMat[index1][index2-1]
		
	inputMat[1][1] = enumerateMat[index1][index2]
	
	try:
		inputMat[1][2] = enumerateMat[index1][index2+1]
	except:
		pass
	try:
		if y:
			inputMat[2][0] = enumerateMat[index1+1][index2-1]
	except:
		pass
	try:
		inputMat[2][1] = enumerateMat[index1+1][index2]
	except:
		pass
	try:
		inputMat[2][2] = enumerateMat[index1+1][index2+1]
	except:
		pass

	return inputMat



temp = changeToBinary(img_array, 128)

# shrinked = shrinkImage(temp)
shrinked = temp.copy()



def markedMatrix(inputMat):

	markedImge = np.zeros(shape=inputMat.shape)
	for index1, row in enumerate(inputMat):
		for index2, ele in enumerate(row):
			if ele == 1:
				matrix = getMatrix(inputMat, index1, index2)
				matrix[0][0]=0
				matrix[2][0]=0
				matrix[0][2]=0
				matrix[2][2]=0
				matrix[1][1]=0					
				if 1 in matrix:
					markedImge[index1][index2] = 1
	return markedImge

def connectShrinkProcess(inputMat, markedImge):
	for index1, row in enumerate(inputMat):
		for index2, ele in enumerate(row):
			matrix = getMatrix(inputMat, index1, index2)
			if markedImge[index1][index2]==1:
				if yokoiNumber(matrix)==1:	
					inputMat[index1][index2] = 0
	return inputMat

for epcho in range(20):
	print(epcho)
	yokoiImage = np.zeros(shape=shrinked.shape)
	for row in range(64):
		for col in range(64):
			kernel = getMatrix(shrinked, row, col, padding="zeros")  
			yokoiImage[row][col] = yokoiNumber(kernel)
	
	marked = markedMatrix(yokoiImage)
	shrinked = connectShrinkProcess(shrinked, marked)

plt.imshow(shrinked, cmap='gray')
plt.show()















