import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import cv2

img = Image.open('lena.bmp').convert('L')
img_array = np.array(img)


def negative(img):
	range_ =int(np.max(img)-np.min(img))
	max_ = np.max(img)
	for index1, row in enumerate(img):
		for index2, ele in enumerate(row):
			img[index1][index2] = int(max_ - ele)
	return img


def normalization(image, threshold):
	max_ = np.max(image)
	for index1, row in enumerate(image):
		for index2, ele in enumerate(row):
			image[index1][index2] = ele/max_ * 255

	image = negative(image)

	drop = image > 255 - threshold
	image *= drop
	for index1, row in enumerate(image):
		for index2, ele in enumerate(row):
			if ele > 0:
				image[index1][index2] = 255
	return image

def convolved(image, kernel):

	result = np.ones(shape=image.shape)
	for index1, row in enumerate(image):
		for index2, ele in enumerate(row):
			temp = getMatrix(image, index1, index2, kernel.shape[0])
			value = temp * kernel

			result[index1][index2] = np.sum(value)
			
	return result


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



# robert----------------------------------
threshold = 15
temp = np.ones(shape=img_array.shape)
temp *=255
robert_x = np.array([[1, 0],
				   [0, -1]])
robert_y = np.array([[0, 1],
				   [-1, 0]])

filterList = [robert_x, robert_y]
for i in range(temp.shape[0]):
	for j in range(temp.shape[1]):
		kernel = getMatrix(img_array, i, j, 2)
		max_ = 0
		for k in range(2):
			value = np.sum(kernel * filterList[k])
			value /=2
			max_ += value*value
			

		max_ = np.sqrt(max_)
		if max_ > threshold:
			temp[i][j] = 0


# robert = normalization(robert, threshold)
plt.title("robert : threshold = " + str(threshold))
plt.imshow(temp, cmap='gray')
plt.show()




# prewitt----------------------------------
threshold = 40
prewitt_x = np.array([[-1,-1,-1],[0,0,0],[1,1,1]])
prewitt_y = np.array([[-1,0,1],[-1,0,1],[-1,0,1]])
xx = convolved(img_array, prewitt_x)
yy = convolved(img_array, prewitt_y)
prewitt = np.sqrt(xx**2/6 + yy**2/6)
prewitt = normalization(prewitt, 45)
plt.title("prewitt : threshold = " + str(threshold))
plt.imshow(prewitt, cmap='gray')
plt.show()



# sobel----------------------------------
threshold = 40
sobel_x = np.array([[1,0,-1],[2,0,-2],[1,0,-1]])
sobel_y = np.array([[1,2,1],[0,0,0],[-1,-2,-1]])
xx = convolved(img_array, sobel_x)
yy = convolved(img_array, sobel_y)
sobel = np.sqrt(xx**2/8+yy**2/8)
sobel = normalization(sobel, threshold)
plt.title("sobel : threshold = " + str(threshold))
plt.imshow(sobel, cmap='gray')
plt.show()



# frei chen----------------------------------
threshold = 35
m1 = np.array([[1, 2**0.5, 1],[0,0,0],[-1, -2**0.5, -1]])
m2 = np.array([[1, 0, -1],[2**0.5,0,-2**0.5],[1, 0, -1]])

g1 = convolved(img_array, m1)
g2 = convolved(img_array, m2)


frei_chen_1 = np.sqrt((g1**2/8 + g2**2/8))


frei_chen_1 = normalization(frei_chen_1, threshold)
plt.title("frei_chen : threshold = " +str(threshold))
plt.imshow(frei_chen_1, cmap='gray')
plt.show()


# kirsch---------------------------------- 
threshold = 120
m1 = np.array([ [5, 5, 5],
			    [-3, 0, -3],
			    [-3,-3,-3] ])

m2 = np.array([ [5,5,-3],
				[5,0,-3],
				[-3,-3,-3] ])

m3 = np.array([ [5,-3,-3],
				[5,0,-3],
				[5,-3,-3]])

m4 = np.array([ [-3,-3,-3],
				[5,0,-3],
				[5,5,-3]])

m5 = np.array([ [-3,-3,-3],
				[-3,0,-3],
				[5,5,5]])

m6 = np.array([ [-3,-3,-3],
				[-3,0,5],
				[-3,5,5]])

m7 = np.array([ [-3,-3,5],
				[-3,0,5],
				[-3,-3,5]])

m8 = np.array([ [-3,5,5],
				[-3,0,5],
				[-3,-3,-3]])

g1 = convolved(img_array, m1)
g2 = convolved(img_array, m2)
g3 = convolved(img_array, m3)
g4 = convolved(img_array, m4)
g5 = convolved(img_array, m5)
g6 = convolved(img_array, m6)
g7 = convolved(img_array, m7)
g8 = convolved(img_array, m8)

kirsch = np.zeros(shape=img_array.shape)
for i in range(kirsch.shape[0]):
	for j in range(kirsch.shape[1]):
		kirsch[i][j] = np.max([g1[i][j], g2[i][j], g3[i][j], g4[i][j], g5[i][j], g6[i][j], g7[i][j], g8[i][j]])

value = (kirsch/np.sqrt(15)) > threshold
kirsch *= value
plt.title("kirsch : threshold = " +str(threshold))
plt.imshow(kirsch, cmap='binary', vmin=0, vmax=255)
plt.show()



# robinson---------------------------------- 
threshold = 43
m1 = np.array([ [ 1, 0,-1],
			    [ 2, 0,-2],
			    [ 1, 0,-1] ])

m2 = np.array([ [ 0,  1, 2],
				[ -1, 0, 1],
				[ -2,-1, 0] ])

m3 = np.array([ [ 1, 2, 1],
				[ 0, 0, 0],
				[-1,-2,-1]])

m4 = np.array([ [ 2, 1, 0],
				[ 1, 0,-1],
				[ 0,-1,-2]])


g1 = convolved(img_array, m1)
g2 = convolved(img_array, m2)
g3 = convolved(img_array, m3)
g4 = convolved(img_array, m4)


robinson = np.ones(shape=img_array.shape)
robinson *= 255
for i in range(robinson.shape[0]):
	for j in range(robinson.shape[1]):
		value = np.max([g1[i][j], g2[i][j], g3[i][j], g4[i][j]])
		value /= np.sqrt(6)
		if value > threshold:
			robinson[i][j] = 0
plt.title("robinson : threshold = " +str(threshold))
plt.imshow(robinson, cmap='gray', vmin=0, vmax=255)
plt.show()

# Nevatia & Babu---------------------------------- 
threshold = 13500
m1 = np.array([ [100, 100, 100, 100, 100],
				[100, 100, 100, 100, 100],
				[  0,   0,   0,   0,   0,],
				[-100,-100,-100,-100,-100],
				[-100,-100,-100,-100,-100]])

m2 = np.array([ [100, 100, 100, 100, 100],
				[100, 100, 100,  78, -32],
				[100,  92,   0, -92,-100],
				[ 32, -78,-100,-100,-100],
				[-100,-100,-100,-100,-100]])

m3 = np.array([ [100, 100, 100, 32, 100],
				[100, 100,  92,-78,-100],
				[100, 100,   0,-100,-100],
				[100,  78, -92,-100,-100],
				[100, -32,-100,-100,-100]])

m4 = np.array([ [-100,-100,   0, 100, 100],
				[-100,-100,   0, 100, 100],
				[-100,-100,   0, 100, 100],
				[-100,-100,   0, 100, 100],
				[-100,-100,   0, 100, 100]])

m5 = np.array([ [-100,  32, 100, 100, 100],
				[-100, -78,  92, 100, 100],
				[-100,-100,   0, 100, 100],
				[-100,-100, -92,  78, 100],
				[-100,-100,-100, -32, 100]])

m6 = np.array([ [ 100, 100, 100, 100, 100],
				[ -32,  78, 100, 100, 100],
				[-100, -92,   0,  92, 100],
				[-100,-100,-100, -78,  32],
				[-100,-100,-100,-100,-100]])



filterList = [m1, m2, m3, m4, m5, m6]

Babu = np.ones(shape=img_array.shape)
Babu *= 255

for i in range(Babu.shape[0]):
	for j in range(Babu.shape[1]):
		kernel = getMatrix(img_array, i, j, 5)
		max_ = threshold-1
		for k in range(6):
			value = np.sum(kernel * filterList[k])
			positive = filterList[k] > 0
			num = np.sum(positive)
			value /= np.sqrt(num)
			if value > max_:
				max_ = value

		if max_ > threshold:
			Babu[i][j] = 0
plt.title("nevatia babu : threshold = " +str(threshold))
plt.imshow(Babu, cmap='gray')
plt.show()





