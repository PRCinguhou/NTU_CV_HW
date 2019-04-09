import numpy as np
import matplotlib.pyplot as plt
import cv2
from PIL import Image
import scipy
img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)

def signal_noise_ratio(originalImg, noiseImg):
	row, col = originalImg.shape
	totalpixel = row * col
	sumSrc = 0
	sumDes = 0
	for i in range(row):
		for j in range(col):
			sumSrc+=float(originalImg[i][j])
			sumDes+=float(originalImg[i][j])-float(noiseImg[i][j])
	srcMean = sumSrc/totalpixel
	desMean = sumDes/totalpixel
	sumVs = 0
	sumVn = 0
	for i in range(row):
		for j in range(col):
			sumVs += (float(originalImg[i][j])-float(srcMean)) * (float(originalImg[i][j])-float(srcMean))
			sumVn += (float(originalImg[i][j])-float(noiseImg[i][j])-float(desMean)) * (float(originalImg[i][j])-float(noiseImg[i][j])-float(desMean))

	vstovn = np.sqrt(sumVs/totalpixel)/np.sqrt(sumVn/totalpixel)
	print("SNR = ", 20 * np.log10(vstovn))

	return 20 * np.log10(vstovn)

def findMax(input_, row, col):

	if row>=2:
		top=row
	else:
		top=2

	if col>=2:
		left=col
	else:
		left=2
		
	return input_[top-2:top+3, left-2:left+3].max()
	
def findMin(input_, row, col):

	if row>=2:
		top=row
	else:
		top=2

	if col>=2:
		left=col
	else:
		left=2
		
	return input_[top-2:top+3, left-2:left+3].min()
	

def getMatrix(enumerateMat, index1, index2, kernel_size):
	size = (kernel_size-1)/2
	x = index1 - size >= 0
	x *= (index1 - size)
	
	y = index2 - size >= 0
	y *= (index2 - size)
	
	return enumerateMat[int(x):int(index1+size+1), int(y):int(index2+size+1)]

def gausianNoise(inputMat, amplitude):

	gausian_noise = inputMat.copy()
	for row in range(inputMat.shape[0]):
		for col in range(inputMat.shape[1]):
			value = np.random.normal(0,1)
			gausian_noise[row][col] += value*amplitude
	
	Image.fromarray(gausian_noise).save('gausian-'+'with-'+str(amplitude)+'.jpg')
	return gausian_noise

def salt_and_pepper_noise(inputMat, prob):
	# salt & pepper
	salt_and_pepper = inputMat.copy()
	probMatrix = np.random.uniform(0,1,inputMat.shape[0]*inputMat.shape[1]).reshape(inputMat.shape) 
	# turn to black dot
	pepper = np.ones(shape=inputMat.shape) * (probMatrix > prob)
	salt_and_pepper *= pepper.astype(np.uint8)

	# turn to white dot
	salt = probMatrix > (1-prob)
	for index1, row in enumerate(salt):
		for index2, ele in enumerate(row):
			if ele:
				salt_and_pepper[index1][index2] = 255

	Image.fromarray(salt_and_pepper).save('salt_and_pepper-'+str(prob*100)+'.jpg')
	return salt_and_pepper

def box_filter(inputMat, name, kernel_size):	
	# box filter
	print(name," with box",str(kernel_size)," : ")
	box_filter = inputMat.copy()

	for index1, row in enumerate(box_filter):
		for index2, ele in enumerate(row):
			kernel = getMatrix(box_filter, index1, index2, kernel_size)
			box_filter[index1][index2] = np.mean(kernel)
	Image.fromarray(box_filter).save(name+'-with-'+str(kernel_size)+'box-filter-'+'.jpg')
	return box_filter

def median_filter(inputMat, name, kernel_size):
	print(name," with median",str(kernel_size)," : ")
	median_filter = inputMat.copy()

	for index1, row in enumerate(inputMat):
		for index2, ele in enumerate(row):
			kernel = getMatrix(inputMat, index1, index2, kernel_size)
			median_filter[index1][index2] = np.median(kernel)
	Image.fromarray(median_filter).save(name+'-with-'+str(kernel_size)+'median-filter'+'.jpg')
	
	return median_filter

def dilation(inputMat):
	# dilation		
	dilation = inputMat.copy()
	for index1, row in enumerate(inputMat):
		for index2, ele in enumerate(row):
			x = findMax(inputMat,index1,index2)
			dilation[index1][index2]=x

	return dilation

def erosion(inputMat):	
	# erosion
	erosion = inputMat.copy()	
	for index1, row in enumerate(inputMat):
		for index2, ele in enumerate(row):
			x = findMin(inputMat,index1,index2)
			erosion[index1][index2]=x

	return erosion
	
def openning(inputMat):
	openning = inputMat.copy()
	openning = dilation(erosion(inputMat))
	return openning

def closing(inputMat):
	closing = inputMat.copy()
	closing = erosion(dilation(inputMat))
	return closing





# 4 noise image
gausian_noise_with_10 = gausianNoise(img, 10)
gausian_noise_with_30 = gausianNoise(img, 30)
salt_and_pepper_5 = salt_and_pepper_noise(img, 0.05)
salt_and_pepper_10 = salt_and_pepper_noise(img, 0.1)



print("Gausian10 SNR:")
print(signal_noise_ratio(img, gausian_noise_with_10))
print('\n')

print("Gausian30 SNR:")
print(signal_noise_ratio(img, gausian_noise_with_30))
print('\n')

print("salt_and_pepper_5 SNR:")
print(signal_noise_ratio(img, salt_and_pepper_5))
print('\n')

print("salt_and_pepper_10 SNR:")
print(signal_noise_ratio(img, salt_and_pepper_10))
print('\n')
# box filter with kernel 3

tempImg = box_filter(gausian_noise_with_10, "gausian10", 3)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = box_filter(gausian_noise_with_30, "gausian30", 3)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = box_filter(salt_and_pepper_5, "salt_and_pepper_5", 3)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = box_filter(salt_and_pepper_10, "salt_and_pepper_10", 3)
print(signal_noise_ratio(img, tempImg))
print('\n')

# box filter with kernel 5
tempImg = box_filter(gausian_noise_with_10, "gausian10", 5)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = box_filter(gausian_noise_with_30, "gausian30", 5)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = box_filter(salt_and_pepper_5, "salt_and_pepper_5", 5)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = box_filter(salt_and_pepper_10, "salt_and_pepper_10", 5)
print(signal_noise_ratio(img, tempImg))
print('\n')

#median filter with 3
tempImg = median_filter(gausian_noise_with_10, 'gausian10', 3)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = median_filter(gausian_noise_with_30, 'gausian30', 3)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = median_filter(salt_and_pepper_5, 'salt_and_pepper_5', 3)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = median_filter(salt_and_pepper_10, 'salt_and_pepper_10', 3)
print(signal_noise_ratio(img, tempImg))
print('\n')

#median filter with 5
tempImg = median_filter(gausian_noise_with_10, 'gausian10', 5)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = median_filter(gausian_noise_with_30, 'gausian30', 5)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = median_filter(salt_and_pepper_5, 'salt_and_pepper_5', 5)
print(signal_noise_ratio(img, tempImg))
print('\n')

tempImg = median_filter(salt_and_pepper_10, 'salt_and_pepper_10', 5)
print(signal_noise_ratio(img, tempImg))
print('\n')

#openning then closing
print("gausian10_open_close")
tempImg = closing(openning(gausian_noise_with_10))
print(signal_noise_ratio(img, tempImg))
print('\n')
Image.fromarray(tempImg).save('gausian10_open_close.jpg')

print("gausian30_open_close")
tempImg = closing(openning(gausian_noise_with_30))
print(signal_noise_ratio(img, tempImg))
print('\n')
Image.fromarray(tempImg).save('gausian30_open_close.jpg')

print("salt_and_pepper_5_open_close")
tempImg = closing(openning(salt_and_pepper_5))
print(signal_noise_ratio(img, tempImg))
print('\n')
Image.fromarray(tempImg).save('salt_and_pepper_5_open_close.jpg')

print("salt_and_pepper_10_open_close")
tempImg = closing(openning(salt_and_pepper_10))
print(signal_noise_ratio(img, tempImg))
print('\n')
Image.fromarray(tempImg).save('salt_and_pepper_10_open_close.jpg')

#closing then openning
print("gausian10_close_open")
tempImg = openning(closing(gausian_noise_with_10))
print(signal_noise_ratio(img, tempImg))
print('\n')
Image.fromarray(tempImg).save('gausian10_close_open.jpg')

print("gausian30_close_open")
tempImg = openning(closing(gausian_noise_with_30))
print(signal_noise_ratio(img, tempImg))
print('\n')
Image.fromarray(tempImg).save('gausian30_close_open.jpg')

print("salt_and_pepper_5_close_open")
tempImg = openning(closing(salt_and_pepper_5))
print(signal_noise_ratio(img, tempImg))
print('\n')
Image.fromarray(tempImg).save('salt_and_pepper_5_close_open.jpg')


print("salt_and_pepper_10_close_open")
tempImg = openning(closing(salt_and_pepper_10))
print(signal_noise_ratio(img, tempImg))
print('\n')
Image.fromarray(tempImg).save('salt_and_pepper_10_close_open.jpg')





