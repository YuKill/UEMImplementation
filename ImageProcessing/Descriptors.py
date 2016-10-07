#! /usr/bin/python

import numpy as n
import cv2
import sys

from math import *
from queue import *

def fourier(Vec, Freq):
    Exp = -2 * pi * Freq / len(Vec)
    Each = [Vec[I] * (cos(Exp * I) + (sin(Exp * I) * 1j)) for I in range(len(Vec))]
    return sum(Each) / len(Vec)

def invFourier(Vec, M, Top):
    Exp = 2 * pi * M / len(Vec)
    Each = [Vec[I] * (cos(Exp * I) + (sin(Exp * I) * 1j)) for I in range(Top)]
    #Each = [Vec[abs(I-int(Top/2.0))] * (cos(Exp * (I-int(Top/2.0))) + sin(Exp * (I-int(Top/2.0))) * 1j) for I in range(Top)]
    return sum(Each)

def distance(P1, P2):
    return abs(P1[0] - P2[0]) + abs(P1[1] - P2[1])

def reorganize(Positions, Last):
    Proc = {}
    for I in range(len(Positions[0])):
        Cur = n.ravel(Positions[:, I])
        if (not Cur[0] in Proc):
            Line = Positions[:, Positions[0] == Cur[0]]
            Dist = distance(Last, Cur)
            IDist = distance(Last, n.ravel(Line[:, ::-1][:, 0]))
            if (IDist < Dist):
                Positions[:, Positions[0] == Cur[0]] = Line[:, ::-1]
            Proc[Cur[0]] = True
        Last = Cur
    return Positions


def clockWise(Edges):
    Center = n.argwhere(Edges == 255)
    Center = Center.sum(0) / len(Center)
    Border = n.array(n.where(Edges == 255))

    print(Center)
    Left = Border[:, Border[1] <= Center[1]]
    Left = Left[:, n.argsort(Left[0])[::-1]]
    Right = Border[:, Border[1] > Center[1]]
    Right = Right[:, n.argsort(Right[0])]


    print(Left)
    Left = reorganize(Left, n.ravel(Right[:, ::-1][:, 0]))
    print(Left)
    Right = reorganize(Right, n.ravel(Left[:, ::-1][:, 0]))

    return n.concatenate((Right, Left), 1)

ImgName = sys.argv[1]
DescNum = int(sys.argv[2])
#Threshold = int(sys.argv[3])


Img = cv2.imread(ImgName, 0)
Img[Img > 127] = 255;
Img[Img <= 127] = 0;
Img[Img == 255] = 50;
Img[Img == 0] = 255;
Img[Img == 50] = 0;

print(Img.shape)

#Scale = int(Img.shape[0] / 500)
#Img = Img[::Scale, ::Scale]
Width, Height = Img.shape


print(Img)
Edges = cv2.Canny(Img, Width, Height)
#Edges = n.where(Edges == 255)
IEdges = n.argwhere(Edges == 255)
Edges = clockWise(Edges)
#Edges = Edges[::-1]
'''
for I in IEdges:
    Found = Edges[:, Edges[:, Edges[0] == I[0]][1] == I[1]]
    if (Found.size == 0):
        print(Edges[:, Edges[0] == I[0]])
        print(Edges[:, Edges[:, Edges[0] == I[0]][1] == I[1]])
        print(I)

String = ""
for I in range(len(Edges[0])):
    String = String + str(Edges[:, I])
print(String)
print(Edges)
'''

Complex = n.zeros(Img.shape, n.complex)
Complex = [Edges[0][I] + Edges[1][I] * 1j for I in range(len(Edges[0]))]
print(len(Complex))


'''
FourierField = [fourier(Complex, I) for I in range(len(Complex))]
print(FourierField[Threshold::])
FourierField[Threshold:] = n.repeat(0 + 0j, len(FourierField[Threshold:]))
print(len(FourierField))
Edges = [invFourier(FourierField, I, DescNum) for I in range(len(FourierField))]
print(len(Edges))
'''
FromZeroT = floor(DescNum / 2)
ToZeroT = len(Complex) - FromZeroT
if (DescNum % 2 == 1):
    ToZeroT = ToZeroT-1

Fourier = n.fft.fft(Complex)
Fourier[FromZeroT:ToZeroT] = 0
print(Fourier[FromZeroT:ToZeroT])
print(FromZeroT, ":", ToZeroT)
IFourier = n.fft.ifft(Fourier)


Contour = n.zeros(Img.shape)
for I in IFourier:
    X = int(I.real)
    Y = int(I.imag)
    if (Y < Height) and (X < Width) and (Y >= 0) and (X >= 0):
        Contour[X, Y] = 255

Contour = n.uint8(Contour)

cv2.imshow(ImgName, Img)
cv2.imshow("Contour.png", Contour)
cv2.waitKey(0)
cv2.destroyAllWindows()
