#
# Take panoramic photos with ESP32-CAM and Python
#
# Replace url variable with the address of your ESP32-CAM
#
# Github: https://github.com/gsampallo/PanoramicCam
# Blog: www.gsampallo.com/blog/
# Twitter @gsampallo
#
from PIL import Image
import requests
from io import BytesIO
import os, sys
import datetime
import cv2
import json

#Define ESP32-CAM's url and the urls for each function
url = "http://192.168.1.111/"
info = url + "info"
right = url + "moverDerecha"
left = url + "moverIzquierda"
cam = url + "cam"

#Number of de photos
nPhoto = 16

#Temp path
temp = "temp/"
#Output path
output = "output/"

#Take the date and time for the filename
def getFileName():
    datetime_object = datetime.datetime.now()
    print(datetime_object)
    d1 = str(datetime_object)
    output = d1.replace(":","")
    output = output.replace(" ","_")
    output = output[0:17]+".jpg"
    return output

#According to the position move the camera to home position
def moveHome():
    response = requests.get(info)
    data = json.loads(response.content.decode())

    pos = int(data["pos"])
    print(pos)
    
    while(pos > 0):
        response = requests.get(left)
        data = json.loads(response.content.decode())

        pos = int(data["pos"])     

#Move the camera to right
def moveRight():
    response = requests.get(right)
    data = json.loads(response.content.decode())

    pos = int(data["pos"])
    print(pos)    

#Move the camera to left
def moveLeft():
    response = requests.get(left)
    data = json.loads(response.content.decode())

    pos = int(data["pos"])
    print(pos)    

def takePhoto(n): #Take the photo, rotate 270 degrees and save in temp folder

    response = requests.get(cam)
    img = Image.open(BytesIO(response.content))
    transposed  = img.transpose(Image.ROTATE_270) 
    transposed.save(temp+str(n)+".jpg")   


moveHome()

n = 0
while(n < nPhoto):
    takePhoto(n)
    moveRight()
    n += 1

#Read each of the images and then put it in the array
n = 0
imgs = []
while(n < nPhoto):
    archivo = "temp/"+str(n)+".jpg"
    print(archivo)
    img = cv2.imread(archivo)
    imgs.append(img)
    n += 1

#Stitch the images on the array, and save the output image. 
stitcher = cv2.Stitcher.create(cv2.Stitcher_PANORAMA)
status, pano = stitcher.stitch(imgs)

if status != cv2.Stitcher_OK:
    print("Can't stitch images, error code = %d" % status)
    sys.exit(-1)

cv2.imwrite(output+getFileName(), pano);

# On OpenCV 3.3 use
#stitcher = cv2.createStitcher(True)
#result = stitcher.stitch(imgs)
#cv2.imwrite(output+getFileName(), result[1])