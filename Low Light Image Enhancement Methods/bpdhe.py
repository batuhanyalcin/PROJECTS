import cv2


input = cv2.imread("../Test Inputs/2015_00001.png")
def bpdhe(img):
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    return hsv

out = bpdhe(input)