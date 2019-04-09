import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import cv2

img = Image.open('lena.bmp').convert('L')
img_array = np.array(img)

def getMatrix(enumerateMat, index1, index2, kernel_size):

	if kernel_size == 2:
		inputMat = np.zeros(shape=(kernel_size, kernel_size))
		if index1 >= kernel_size-1:
			inputMat[0][0] = enumerateMat[index1-1][index2]
			
		inputMat[1][0] = enumerateMat[index1][index2]

		if index2 < enumerateMat.shape[1]-1:
			inputMat[1][1] = enumerateMat[index1][index2+1]
		
		try:
			if index1 > 0:
				inputMat[0][1] = enumerateMat[index1-1][index2+1]
				
		except:
			pass

	else:
		inputMat = np.zeros(shape=(kernel_size,kernel_size))

		left = int(-1*(kernel_size-1)/2)
		right= int((kernel_size-1)/2)

		for i in range(left,right+1):
			for j in range(left,right+1):
				if index1 + i >= 0 and index1 + i < enumerateMat.shape[0]:
					if index2 + j >= 0 and index2 + j < enumerateMat.shape[1]:
						inputMat[i+right][j+right] = enumerateMat[index1+i][index2+j]	

	return inputMat

def laplacian(img, threshold, mask, maskSize):
	

	temp = np.zeros(shape=img.shape)
	result = np.ones(shape=img.shape)
	result *= 255
	bias = int((maskSize-1)/2)
	for i in range(bias, img.shape[0]-bias):
		for j in range(bias, img.shape[1]-bias):
			temp[i][j] = np.sum(getMatrix(img, i-bias, j-bias, maskSize) * mask)
	
	for i in range(img.shape[0]-1):
		for j in range(img_array.shape[1]-1):
			if(temp[i][j] > threshold):
				for x in range(-1,2):
					for y in range(-1,2):
						if(temp[i+x][j+y] < -1*threshold):
							result[i][j] = 0

	return result

def generateDOG(kernelSize, sigma1, sigma2):
	kernel = np.zeros(shape=(kernelSize, kernelSize))
	mean = 0
	for x in range(int(-1*(kernelSize-1)/2), int((kernelSize+1)/2)):
		for y in range(int(-1*(kernelSize-1)/2), int((kernelSize+1)/2)):
			a = (1/(2*np.pi*sigma1*sigma1)) * np.exp((-(x*x+y*y)/(2*sigma1*sigma1)))
			b = (1/(2*np.pi*sigma2*sigma2)) * np.exp((-(x*x+y*y)/(2*sigma2*sigma2)))
			kernel[x+5][y+5] = a-b
			mean+=a-b
	mean /= kernelSize*kernelSize
	for x in range(kernelSize):
		for y in range(kernelSize):
			kernel[x][y] -= mean

	return kernel


laplacianMask1 = np.array([[ 0, 1, 0],
						   [ 1,-4, 1],
						   [ 0, 1, 0]])

laplacianMask2 = np.array([[ 1/3, 1/3, 1/3],
						   [ 1/3,-8/3, 1/3],
						   [ 1/3, 1/3, 1/3]])

laplacianMask3 = np.array([[ 2/3, -1/3, 2/3],
						   [ -1/3,-4/3, -1/3],
						   [ 2/3, -1/3, 2/3]])

laplacianMask4 = np.array([[0, 0,  0, -1, -1, -2, -1, -1,  0,  0,  0],
                           [0, 0, -2, -4, -8, -9, -8, -4, -2,  0,  0],
                           [0,-2, -7,-15,-22,-23,-22,-15, -7, -2,  0],
                           [-1,-4,-15,-24,-14, -1,-14,-24,-15, -4, -1],
                           [-1,-8,-22,-14, 52,103, 52,-14,-22, -8, -1],
                           [-2,-9,-23, -1,103,178,103, -1,-23, -9, -2],
                           [-1,-8,-22,-14, 52,103, 52,-14,-22, -8, -1],
                           [-1,-4,-15,-24,-14, -1,-14,-24,-15, -4, -1],
                           [0,-2, -7,-15,-22,-23,-22,-15, -7, -2,  0],
                           [0, 0, -2, -4, -8, -9, -8, -4, -2,  0,  0],
                           [0, 0,  0, -1, -1, -2, -1, -1,  0,  0,  0]])

print(np.sum(laplacianMask4))
print(np.mean(laplacianMask4))

laplacianMask5 = generateDOG(11, 1, 3)




laplacianImg1 = laplacian(img_array, 15, laplacianMask1, 3)
laplacianImg2 = laplacian(img_array, 12, laplacianMask2, 3)
laplacianImg3 = laplacian(img_array, 12, laplacianMask3, 3)
laplacianImg4 = laplacian(img_array, 1900, laplacianMask4, 11)
laplacianImg5 = laplacian(img_array, 2, laplacianMask5, 11)


plt.imshow(laplacianImg1, cmap='gray')
plt.show()
plt.imshow(laplacianImg2, cmap='gray')
plt.show()
plt.imshow(laplacianImg3, cmap='gray')
plt.show()
plt.imshow(laplacianImg4, cmap='gray')
plt.show()
plt.imshow(laplacianImg5, cmap='gray')
plt.show()