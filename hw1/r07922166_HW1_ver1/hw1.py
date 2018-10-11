import numpy as np
import matplotlib.pyplot as plt
import cv2


img = cv2.imread('lena.bmp', cv2.IMREAD_GRAYSCALE)

# histogram 
hist_y = [0 for i in range(255)]
hist_x = [i for i in range(255)]

for row in img:
	for ele in row:
		hist_y[ele]+=1

# plt.bar(hist_x, hist_y,width=0.5)
# plt.show()

# thrshold
thresh_img = img
for index1, row in enumerate(img):
	for index2, ele in enumerate(row):
		if ele > 127:
			img[index1][index2]=255
		else:
			img[index1][index2]=0

# plt.imshow(thresh_img, cmap='gray')
# plt.show()





# connect component
temp = np.zeros(shape=thresh_img.shape, dtype=np.int32)
for index1, r in enumerate(thresh_img):
	for index2, ele in enumerate(r):
		temp[index1][index2]=ele

eq = [ set() for i in range(3000)]


# label
label = 1
print("start labeling")
for index1, r in enumerate(temp):
	for index2, ele in enumerate(r):
		if ele != 0:
			behind=0
			top=0
			if index2 > 0:
				behind = temp[index1][index2-1]	
			if index1 > 0:
				top = temp[index1-1][index2]
			
			if behind ==0 and top ==0:
				temp[index1][index2]=label
				eq[label].add(label)
				label+=1
			# confilct
			elif behind != 0 and top != 0:
				temp[index1][index2]=min([behind, top])
				eq[min([behind, top])].add(max([behind, top]))
			else:
				temp[index1][index2]=max([behind, top])


print("delete unlabeled set")
# delete unlabeled set
eq = [i for i in eq if len(i)>0]




# find equivalence set
print("start finding eqivalence set")
equivalenceList = []
for i in eq:
	set_ = set(i)
	for j in eq:
		if i!=j and len(i.intersection(j))!=0:
			for ele in j:
				set_.add(ele)
	check = 0
	for tmp in equivalenceList:
		if set_.issubset(tmp):
			check =1
			break
		elif tmp.issubset(set_):
			equivalenceList.remove(tmp)
		elif len(tmp.intersection(set_))!=0:
			set_ = set_.union(tmp)
	if check == 0:
		equivalenceList.append(set_)




print("equivalence labling")
# change all eqivalence into same number

label_dict = {}
label = 1
for set_ in equivalenceList:
	for num in set_:
		label_dict[num]=label
	label+=1


for index1, i in enumerate(temp):
	for index2, j in enumerate(i):
		try:
			temp[index1][index2]=label_dict[j]
		except:
			temp[index1][index2]=0



print("omit under 500 ")
labelList=[0 for i in range(label+1)]

for index1, i in enumerate(temp):
	for index2, j in enumerate(i):			
		labelList[j]+=1

temp_list=[]
for index, i in enumerate(labelList):
	if i>=500 and index!=0:
		temp_list.append(index)
labelList=temp_list






print("find bounding box")
# 找出bounding box的邊界
bounding_box = {}
for label in labelList:
	# top, bottom, left, right
	bounding_box[label]=[1000,0,1000,0]

for index1, i in enumerate(temp):
	for index2, j in enumerate(i):			
		# 若是為超過500數量的label，才需要畫bounding box
		if j in labelList:
			# index1決定top,bottom,  index2決定left,right
			if index1 < bounding_box[j][0]:
				bounding_box[j][0]=index1
			if index1 > bounding_box[j][1]:
				bounding_box[j][1]=index1
			if index2 < bounding_box[j][2]:
				bounding_box[j][2] = index2
			if index2 > bounding_box[j][3]:
				bounding_box[j][3] = index2


# 
for index1, i in enumerate(temp):
	for index2, j in enumerate(i):			
		if j in labelList:
			temp[index1][index2]=255
		else:
			temp[index1][index2]=0


finalImg = thresh_img.copy()
finalImg = finalImg.astype(np.uint8)
finalImg = cv2.cvtColor(finalImg, cv2.COLOR_GRAY2RGB)
# 參數：(img, (top-left), (right-bottom), color)
for box in bounding_box:
	print(box)
	top = bounding_box[box][0]
	bottom = bounding_box[box][1]
	left = bounding_box[box][2]
	right = bounding_box[box][3]
	cv2.rectangle(finalImg, (left, top), (right, bottom), (0,0,255),3)
	cv2.circle(finalImg,(int((left+right)/2),int((top+bottom)/2)), 5, (255,0,0), -1)
	


plt.imshow(finalImg)
plt.show()

