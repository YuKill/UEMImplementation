import numpy as np
import cv2

imgArray = cv2.imread('Blob.png', 0)

numDescs = int(input("Numero de descritores > "))

allEdges = cv2.Canny(imgArray, imgArray.shape[0], imgArray.shape[1])
objEdges = np.where(allEdges == 255)

complexEdges = np.zeros(len(objEdges), dtype = np.complex_)
complexEdges = objEdges[0] + (objEdges[1] * 1j)

descResult = np.complex_(np.zeros(len(complexEdges)))

for i in range(numDescs):
    dft = np.fft.fft(complexEdges)
    descResult += dft
    complexEdges = np.roll(complexEdges, 1)

descResult = descResult/numDescs
output = np.fft.ifft(descResult)

blackBgImg = np.zeros(imgArray.shape)
blackBgImg[np.uint8(output.real), np.uint8(output.imag)] = 255

blackBgImg = np.uint8(blackBgImg)

cv2.imshow("Contour", blackBgImg)
cv2.waitKey(0)
cv2.destroyAllWindows()
