FROM ubuntu:latest

# Volume creation
VOLUME /home/ImageDetectionServer
WORKDIR /home/ImageDetectionServer

# Install PNG Library
RUN apt-get update
RUN apt-get install libpng16-16
RUN apt-get -y install libpng-dev libssl-dev

# Install Make and GCC
RUN apt-get update
RUN apt-get -y install make
RUN apt-get -y install build-essential

# Add files to volume into container
ADD server.c /home/ImageDetectionServer/server.c
ADD color_detection.c /home/ImageDetectionServer/color_detection.c
ADD Makefile /home/ImageDetectionServer/Makefile