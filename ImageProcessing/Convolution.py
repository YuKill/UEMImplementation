#! /usr/bin/python

import cv2
import numpy as n

import sys
from math import *

Img = cv2.imread(sys.argv[1], 0)

#Img = n.array([[1, 1, 1], [1, 1, 1], [1, 1, 1]])

def convolve(Img, G):
    Height, Width = Img.shape
    Res = n.zeros((Height, Width))
    ICopy = n.zeros((Height+2, Width+2))
    InsideW = int((ICopy.size/(Width+2))-1)
    InsideH = int((ICopy.size/(Height+2))-1)
    print(InsideW, InsideH)
    ICopy[1:InsideW, 1:InsideH] = Img
    Idx = [-1, 0, 1]
    for I in Idx:
        for J in Idx:
            Ix = I + 1
            Jx = J + 1
            print("Res:", Res)
            print("ICopy: ", ICopy[Ix:(InsideW+I), Jx:(InsideH+J)])
            Res += ICopy[Ix:(InsideW+I), Jx:(InsideH+J)] * G[Ix, Jx]

    return Res

def normalize(Img):
    Max = n.max(Img)
    Min = n.min(Img)
    if (Max > 255) or (Min < 0):
        Img = (Img - Min)/(Max - Min)
        Img = Img * 256
        return n.uint8(Img)
    return Img

# Sobel
G = n.array([
    [ 1, 2, 1],
    [ 0, 0, 0],
    [ -1, -2, -1]
    ])
Conv = convolve(Img, G)
Conv = normalize(Conv)

G = n.array([
    [ 1, 0, -1],
    [ 2, 0, -2],
    [ 1, 0, -1]
    ])
Conv = convolve(Conv, G)
Conv = normalize(Conv)

'''

# Sharpen
G = n.array([
    [ 0, -1, 0],
    [ -1, 5, -1],
    [ 0, -1, 0]
    ])

# Gaussian
G = n.array([
    [ 1, 2, 1],
    [ 2, 4, 2],
    [ 1, 2, 1]
    ])

# Box blur
G = n.array([
    [ 1, 1, 1],
    [ 1, 1, 1],
    [ 1, 1, 1]
    ])

# Laplace
G = n.array([
    [ -1, -1, -1],
    [ -1, 8, -1],
    [ -1, -1, -1]
    ])
G = n.array([
    [ 0, 1, 0],
    [ 1, -4, 1],
    [ 0, 1, 0]
    ])

G = n.array([
    [ -1, 2, -1],
    [ -1, 2, -1],
    [ -1, 2, -1]
    ])

'''

print("Conv: ", Conv)
print("Img: ", Img)

print("Hist: ", n.histogram(Conv))

cv2.imshow("Img", Img)
cv2.imshow("Covoluted", n.uint8(Conv > 119)*255)
cv2.waitKey(0)
cv2.destroyAllWindows()
