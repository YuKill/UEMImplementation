#! /usr/bin/python

import cv2
import numpy as n

import sys

def toHsv(Img): 
    return cv2.cvtColor(Img, cv2.COLOR_RGB2HSV)

def toRgb(Img): 
    return cv2.cvtColor(Img, cv2.COLOR_HSV2RGB)

def toBgr(Img): 
    return cv2.cvtColor(Img, cv2.COLOR_HSV2BGR)

def newPalleta(Color1, Color2):
    Palleta = n.zeros((256, 1, 3))
    Palleta[:,0,0] = n.linspace(Color1[0], Color2[0], 256)
    Palleta[:,0,1] = n.linspace(Color1[1], Color2[1], 256)
    Palleta[:,0,2] = n.linspace(Color1[2], Color2[2], 256)
    return Palleta

def paintWithPalleta(Img, Palleta):
    Height, Width = Img.shape
    Raveled = Img.ravel()
    NewImg = n.zeros((Height, Width, 3))
    NewImg[:,:,0] = n.reshape([Palleta[I, 0, 0] for I in Raveled], (Height, Width))
    NewImg[:,:,1] = n.reshape([Palleta[I, 0, 1] for I in Raveled], (Height, Width))
    NewImg[:,:,2] = n.reshape([Palleta[I, 0, 2] for I in Raveled], (Height, Width))
    NewImg = n.uint8(NewImg)
    return NewImg

ImageName = sys.argv[1]
Img = cv2.imread(ImageName, 0)

Color1 = sys.argv[2].split(":")
Color1[0] = int(Color1[0], 16)
Color1[1] = int(Color1[1], 16)
Color1[2] = int(Color1[2], 16)
Color1Hsv = toHsv(n.uint8(n.array([[[Color1[0], Color1[1], Color1[2]]]]))).ravel()
print(Color1)
print(Color1Hsv)

Color2 = sys.argv[3].split(":")
Color2[0] = int(Color2[0], 16)
Color2[1] = int(Color2[1], 16)
Color2[2] = int(Color2[2], 16)
Color2Hsv = toHsv(n.uint8(n.array([[[Color2[0], Color2[1], Color2[2]]]]))).ravel()
print(Color2)
print(Color2Hsv)

Palleta = newPalleta(Color1[::-1], Color2[::-1])
PalletaImg = n.uint8(n.tile(Palleta, (1, 256, 1)))
NewImg = paintWithPalleta(Img, Palleta)

PalletaHsv = n.uint8(newPalleta(Color1Hsv, Color2Hsv))
PalletaImgHsv = toBgr(n.tile(PalletaHsv, (1, 256, 1)))
NewImgHsv = toBgr(paintWithPalleta(Img, PalletaHsv))

cv2.imshow(ImageName, Img)
cv2.imshow("NewImage", NewImg)
cv2.imshow("NewImageHSV", NewImgHsv)
cv2.imshow("Palleta", PalletaImg)
cv2.imshow("PalletaHsv", PalletaImgHsv)
cv2.waitKey(0)
cv2.destroyAllWindows()
