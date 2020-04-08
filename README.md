# PanoramicCam

Take panoramic photos with ESP32-Cam and Python

![Demo](http://www.gsampallo.com/blog/wp-content/uploads/2020/04/panoramic.jpg)

## Installation

1. Install [Docker](https://www.docker.com/get-started)
2. Build docker container:
```
docker build -t espcam .
```
This step must run inside proyect folder.

## How to run
```
docker run -it --rm -v PATH:/app espcam python panoramic.py
```
PATH is the proyect's folder.

The output images will be in output folder.

Example:

```
docker run -it --rm -v C:/Proyects/PanoramicCam:/app espcam python panoramic.py
```

Read the full article on my [blog](http://www.gsampallo.com/blog/?p=644)