import numpy as np
import matplotlib.pyplot as plt
from PIL import Image
img = Image.open('lena.bmp').convert('L')
img_array = np.array(img)



hist = [0 for i in range(256)]
histPDF = [0 for i in range(256)]
histCDF = [0 for i in range(256)]
total = 0

# histogram
for row in img_array:
	for ele in row:
		hist[ele]+=1

# number of pixel
for num in hist:
	total+=num

# PDF
for index, num in enumerate(hist):
	histPDF[index] = num/total

# CDF
for index, p in enumerate(histPDF):
	if index != 0:
		histCDF[index] = histCDF[index-1] + histPDF[index]
	else:
		histCDF[index] = histPDF[index]


plt.imshow(Image.fromarray(img_array))
plt.show()



# histogram equalization
for index1, row in enumerate(img_array):
	for index2, ele in enumerate(row):
		img_array[index1][index2] = round(255*histCDF[ele])

plt.imshow(Image.fromarray(img_array))
plt.show()

result = Image.fromarray(img_array)
result.save('result.jpg')


