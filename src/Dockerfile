#FROM registry.cn-beijing.aliyuncs.com/ailab-paas/cuda10.1-cudnn7-py3.7-ubuntu18.04:v1.0.0.0
FROM registry.cn-beijing.aliyuncs.com/ailab-paas/base_cuda10.2-cudnn8:v0.0.0.1
RUN  #apt-get update &&\
     apt-get install -y libglib2.0-dev && \
     apt-get install -y libboost-dev libboost-filesystem-dev libboost-system-dev libboost-log-dev && \
     apt-get install -y libcurl4-openssl-dev &&\
     apt-get -y install libgomp1 && \
     apt-get -y install libpng-dev && \
     apt-get -y install libtiff-dev

COPY package/ /home/nudity_detect
WORKDIR /home/nudity_detect/bin

RUN echo '/home/nudity_detect/lib' > /etc/ld.so.conf.d/nudity_detect.conf && ldconfig
RUN chmod 777 -Rf /home/nudity_detect/bin/*
RUN chmod 777 -Rf /home/nudity_detect/ai_model/bin/*


ENV LD_LIBRARY_PATH=/home/nudity_detect/lib:${LD_LIBRARY_PATH}
ENV LD_LIBRARY_PATH=/home/nudity_detect/ai_model/lib:${LD_LIBRARY_PATH}
CMD ["./MicroserviceDemo"]
