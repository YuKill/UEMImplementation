#! /usr/bin/python2

import numpy as np
import cv2

from math import *

Img = cv2.imread("Lenna.png", 0)

Img = Img[::-1]
Img = np.tile(Img, (2, 3))

Max = Img.max()
Min = Img.min()
print Max, Min
print Img[Img == 255]

def showImg(Image):
    cv2.imshow("Image", Image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

Hei, Wid = Img.shape

Lines = np.linspace(0, 127, Wid)
Lines = np.tile(Lines, (Hei, 1))
Columns = np.linspace(0, 127, Hei)
Columns = np.tile(Columns.reshape((Hei, 1)), (1, Wid))

InitialColor = Lines + Columns
Contrast = np.absolute(InitialColor - InitialColor[::-1, ::-1])
Contrast = np.full((Hei, Wid), 254) - Contrast

print Contrast

NewImage = np.ravel(Img) * np.ravel(Contrast / 255)
NewImage = np.reshape(NewImage, (Hei, Wid))
NewImage = NewImage + InitialColor
NewImage = np.clip(NewImage, 0, 255)
NewImage = NewImage.astype(np.uint8)
print NewImage
showImg(NewImage)
cv2.imwrite("LennaContrast.png", NewImage)
