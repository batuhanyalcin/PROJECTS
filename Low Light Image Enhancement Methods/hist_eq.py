import cv2
import os

def hist_eq(img):
    YCrCb = cv2.cvtColor(img, cv2.COLOR_BGR2YCrCb)
    Y = img[:, :, 0]
    Y = cv2.equalizeHist(Y)
    YCrCb[:, :, 0] = Y
    out = cv2.cvtColor(YCrCb, cv2.COLOR_YCrCb2BGR)
    return out


def load_images_from_folder(folder):
    #images = []
    for filename in os.listdir(folder):
        img = cv2.imread(os.path.join(folder,filename))
        if img is not None:
            #images.append(img)
            out = hist_eq(img)
            cv2.imwrite(os.path.join("../Test Outputs/HE/",filename), out)
    

load_images_from_folder("../Test Inputs")
