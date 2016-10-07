#! /usr/bin/python

import cv2
import numpy as n
import sys

Name = sys.argv[1]
Gamma = float(sys.argv[2])
Img = cv2.imread(Name, 1)

def gammaCorrect(Img, Gamma, Channel, MaxChannel):
    ChanImg = Img[:, :, Channel]
    Max = n.max(ChanImg)
    Min = n.min(ChanImg)
    RImg = (ChanImg - Min) / (Max - Min)
    RImg = RImg ** Gamma

    RImg = (RImg * (Max - Min)) + Min
    return RImg

NewImg = n.zeros(Img.shape)
NewImg[:, :, 0] = gammaCorrect(Img, Gamma, 0, 255)
NewImg[:, :, 1] = gammaCorrect(Img, Gamma, 1, 255)
NewImg[:, :, 2] = gammaCorrect(Img, Gamma, 2, 255)
print(NewImg.shape)

HSVImg = cv2.cvtColor(Img, cv2.COLOR_BGR2HSV)
HSVImg[:, :, 2] = gammaCorrect(HSVImg, Gamma, 2, 255)
HSVImg = cv2.cvtColor(HSVImg, cv2.COLOR_HSV2BGR)

LABImg = cv2.cvtColor(Img, cv2.COLOR_BGR2LAB)
LABImg[:, :, 0] = gammaCorrect(LABImg, Gamma, 0, 255)
#LABImg[:, :, 1] = gammaCorrect(LABImg, Gamma, 1, 255)
#LABImg[:, :, 2] = gammaCorrect(LABImg, Gamma, 2, 255)
LABImg = cv2.cvtColor(LABImg, cv2.COLOR_LAB2BGR)

NewImg = n.uint8(NewImg)
print("(", n.min(Img), ", ", n.max(Img), ")")
print("(", n.min(NewImg), ", ", n.max(NewImg), ")")


cv2.imshow("Original", Img)
cv2.imwrite("GammaHsv.png", HSVImg)
cv2.imwrite("GammaLAB.png", LABImg)
cv2.imwrite("GammaRGB.png", NewImg)
cv2.waitKey(0)
cv2.destroyAllWindows()
