# PanoramicCam

Take panoramic photos with ESP32-Cam and Python

![Demo](https://www.gsampallo.com/wp-content/uploads/2020/04/panoramic.jpg)

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

Read the article about the software on my [blog](http://www.gsampallo.com/blog/?p=644)

and the article about the harware [blog](http://www.gsampallo.com/blog/?p=644)

Download 3d model [Thingiverse] (https://www.thingiverse.com/thing:4270344)

Youtube video with more details

[![Video](https://img.youtube.com/vi/GJNhcEJuNEQ/0.jpg)](https://www.youtube.com/watch?v=GJNhcEJuNEQ)
