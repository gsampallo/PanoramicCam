from PIL import Image
import requests
from io import BytesIO
import os, sys
import datetime
import cv2



def getFileName():
    datetime_object = datetime.datetime.now()
    print(datetime_object)
    d1 = str(datetime_object)
    output = d1.replace(":","")
    output = output.replace(" ","_")
    output = output[0:17]+".jpg"
    return output


nroFotos = 24

#Cambiar la direccion IP segun su configuracion
url = "http://192.168.1.111/cam"
output = "0.jpg"
response = requests.get(url)
img = Image.open(BytesIO(response.content))
transposed  = img.transpose(Image.ROTATE_270)
try:
    transposed.save(output)
except IOError:
    print("cannot convert", output)

#datetime_object = datetime.datetime.now()
print("imagen 0")

i=1

while i < nroFotos:

    #Cambiar la direccion IP segun su configuracion
    url = "http://192.168.1.111/derecha"
    output = str(i)+".jpg"
    response = requests.get(url)
    img = Image.open(BytesIO(response.content))
    transposed  = img.transpose(Image.ROTATE_270)
    try:
        transposed.save(output)
    except IOError:
        print("cannot convert", output)

    datetime_object = datetime.datetime.now()
    #print(datetime_object)
    print("imagen ",i)
    i = i + 1



stitcher = cv2.createStitcher(True)
nro = 0
imgs = []

while(nro < nroFotos):
    archivo = str(nro)+".jpg"
    print(archivo)
    img = cv2.imread(archivo)
    imgs.append(img)
    nro = nro + 1
    
result = stitcher.stitch(imgs)
cv2.imwrite(getFileName(), result[1])