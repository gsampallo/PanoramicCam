from jjanzic/docker-python3-opencv

ENV TZ=America/Argentina/Buenos_Aires

RUN pip install Pillow
RUN pip install requests

WORKDIR /app

# ENTRYPOINT ["python3"]
# CMD ["ESP360Cam.py"]