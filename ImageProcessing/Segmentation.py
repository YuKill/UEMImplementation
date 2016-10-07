#! /usr/bin/python2

import numpy as np
import cv2

from math import *

SearchParam = 0

def showImg(Images):
    for I in range(len(Images)):
        Name = "Image" + str(I)
        cv2.imshow(Name, Images[I])
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def forEachIdx(Row, Col):
    global Counter
    global Img
    global Objects

    ThisColor = np.max(Objects) + 1
    if Objects[Row][Col] > 0:
        ThisColor = Objects[Row][Col]

    if Row > 0 and Objects[Row-1][Col] == 0:
        Objects[Row-1][Col] = ThisColor
    elif Row > 0:
        Objects[Objects == Objects[Row-1][Col]] = ThisColor

    if Row < ImgRow-1 and Objects[Row+1][Col] == 0:
        Objects[Row+1][Col] = ThisColor
    elif Row < ImgRow-1:
        Objects[Objects == Objects[Row+1][Col]] = ThisColor

    if Col > 0 and Objects[Row][Col-1] == 0:
        Objects[Row][Col-1] = ThisColor
    elif Col > 0:
        Objects[Objects == Objects[Row][Col-1]] = ThisColor

    if Col < ImgCol-1 and Objects[Row][Col+1] == 0:
        Objects[Row][Col+1] = ThisColor
    elif Col < ImgCol-1:
        Objects[Objects == Objects[Row][Col+1]] = ThisColor

def getObject(Id):
    global Objects
    global Images

    if not Id == 0:
        R, C = np.where(Objects == Id)
        Rmin = np.min(R)
        Cmin = np.min(C)
        NRows = np.max(R) - Rmin + 1
        NCols = np.max(C) - Cmin + 1

        NewImage = np.ones(NRows * NCols).reshape(NRows, NCols) * 255
        for J in range(len(R)):
            NewImage[R[J] - Rmin][C[J] - Cmin] = 0

        Images.append(NewImage)

Img = cv2.imread("Test.png", 0)
ImgRow, ImgCol = Img.shape

if np.max(Img) == 1:
    Img = Img * 255
Img[Img > 100] = 255
Img[Img <= 100] = 0

Rows, Cols = np.where(Img == SearchParam)
print len(Rows), len(Cols)

Objects = np.zeros(ImgRow * ImgCol).reshape(ImgRow, ImgCol)
find = np.vectorize(forEachIdx)
find(Rows, Cols)

Images = [Img]
getObjs = np.vectorize(getObject)
getObjs(np.unique(Objects))

'''
Images = [ Img ]
for I in np.unique(Objects):
    if I == 0:
        continue
    R, C = np.where(Objects == I)
    Rmin = np.min(R)
    Cmin = np.min(C)
    NRows = np.max(R) - Rmin + 1
    NCols = np.max(C) - Cmin + 1

    NewImage = np.ones(NRows * NCols).reshape(NRows, NCols) * 255
    for J in range(len(R)):
        NewImage[R[J] - Rmin][C[J] - Cmin] = 0

    Images.append(NewImage)
    
'''
showImg(Images)
