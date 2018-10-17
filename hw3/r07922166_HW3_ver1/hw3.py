import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

img = Image.open('lena.bmp').convert('L')
img_array = np.array(img)


kernel = np.array([[0,1,1,1,0],
				   [1,1,1,1,1],
				   [1,1,1,1,1],
				   [1,1,1,1,1],
				   [0,1,1,1,0]])


J_kernel = np.array([[0,0,0,0,0],
					 [0,0,0,0,0],
					 [1,1,0,0,0],
					 [0,1,0,0,0],
					 [0,0,0,0,0]])

K_kernel = np.array([[0,0,0,0,0],
					 [0,1,1,0,0],
					 [0,0,1,0,0],
					 [0,0,0,0,0],
					 [0,0,0,0,0]])

def negImage(input_):
	for index1, row in enumerate(input_):
		for index2, ele in enumerate(row):
			input_[index1][index2] = 255 - input_[index1][index2]
	return input_

def changeToBinary(input_):
	for index1, row in enumerate(input_):
		for index2, ele in enumerate(row):
			if ele <128:
				input_[index1][index2]=0
			else:
				input_[index1][index2]=1
	return input_

def dilation(input_, kernel, mode="8bit"):
	temp = np.zeros(shape=input_.shape)
	rowCount = input_.shape[0]
	colCount = input_.shape[1]
	kernelCount = int((kernel.shape[0]-1)/2)

	for row in range(kernelCount, rowCount - kernelCount):
		for col in range(kernelCount, colCount - kernelCount):
			if input_[row][col] == 1:
				temp[row - kernelCount:row + kernelCount +1 , col - kernelCount : col + kernelCount +1 ] += kernel

	for index1, row in enumerate(temp):
		for index2, ele in enumerate(row):
			if mode == "8bit":
				if ele > 0:
					temp[index1][index2]=255
				else:
					temp[index1][index2]=0
			elif mode == "1bit":
				if ele > 0:
					temp[index1][index2]=1
				else:
					temp[index1][index2]=0
	return temp

def erosion(input_, kernel, mode="8bit"):
	temp = np.zeros(shape=input_.shape)
	rowCount = input_.shape[0]
	colCount = input_.shape[1]
	kernelCount = int((kernel.shape[0]-1)/2)
	totalScore = sum(sum(kernel))

	for row in range(kernelCount, rowCount - kernelCount):
		for col in range(kernelCount, colCount - kernelCount):
			area = input_[row - kernelCount : row + kernelCount +1 , col - kernelCount : col + kernelCount + 1]
			result = area * kernel
			score = sum(sum(result))
			if mode=="8bit":
				if score == totalScore:
					temp[row][col]=255
				else:
					temp[row][col]=0
			elif mode=="1bit":
				if score == totalScore:
					temp[row][col]=1
				else:
					temp[row][col]=0
	return temp


# dilation & erosion
img_array = changeToBinary(img_array)

DilationResult = dilation(img_array, kernel)
ErosionResult = erosion(img_array, kernel)

plt.imshow(Image.fromarray(DilationResult))
plt.show()

plt.imshow(Image.fromarray(ErosionResult))
plt.show()


# opening & closing
OpeningResult = changeToBinary(ErosionResult)
ClosingResult = changeToBinary(DilationResult)

OpeningResult = dilation(OpeningResult, kernel)
ClosingResult = erosion(ClosingResult, kernel)

plt.imshow(Image.fromarray(OpeningResult))
plt.show()

plt.imshow(Image.fromarray(ClosingResult))
plt.show()

negImg = negImage(np.array(img))
negImg = changeToBinary(negImg)

origin = img_array.copy()

erosionByJ = erosion(origin, J_kernel, mode="1bit")
erosionByK = erosion(negImg, K_kernel, mode="1bit")
finalResult = erosionByJ * erosionByK

for index1, row in enumerate(finalResult):
		for index2, ele in enumerate(row):
			if ele!=0:
				finalResult[index1][index2]=255

plt.imshow(finalResult,cmap='gray')
plt.show()







