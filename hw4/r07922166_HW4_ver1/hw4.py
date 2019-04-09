import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

img = Image.open('lena.bmp').convert('L')
img_array = np.array(img)
dilation = img_array.copy()
erosion = img_array.copy()
kernel = np.array([[0,0,0,0,0],
				   [0,0,0,0,0],
				   [0,0,0,0,0],
				   [0,0,0,0,0],
				   [0,0,0,0,0]])



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
	
# dilation		
for index1, row in enumerate(img_array):
	for index2, ele in enumerate(row):
		x = findMax(img_array,index1,index2)
		dilation[index1][index2]=x

dilation = Image.fromarray(dilation)
dilation.save('Dilation11.jpg')

# erosion	
for index1, row in enumerate(img_array):
	for index2, ele in enumerate(row):
		x = findMin(img_array,index1,index2)
		erosion[index1][index2]=x

erosion = Image.fromarray(erosion)
erosion.save('Erosion11.jpg')


opening = img_array.copy()
for index1, row in enumerate(np.array(erosion)):
	for index2, ele in enumerate(row):
		x = findMax(np.array(erosion),index1,index2)
		opening[index1][index2]=x
opening = Image.fromarray(opening)
opening.save('opening11.jpg')

closing = img_array.copy()
for index1, row in enumerate(np.array(dilation)):
	for index2, ele in enumerate(row):
		x = findMin(np.array(dilation),index1,index2)
		closing[index1][index2]=x
closing = Image.fromarray(closing)
closing.save('closing11.jpg')


