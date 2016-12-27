FROM ubuntu:16.04
RUN apt-get update && apt-get upgrade -y
RUN apt-get install git cmake g++ libboost-system-dev libboost-filesystem-dev libwebsocketpp-dev -y
RUN mkdir /bottleship
RUN mkdir /bottleship/bin
RUN mkdir /bottleship/build
RUN git clone https://github.com/google/googletest.git /bottleship/googletest
RUN git clone https://github.com/google/leveldb.git /bottleship/leveldb
WORKDIR /bottleship/build
COPY . /bottleship
RUN cmake ..
RUN make
RUN cp bottleship /bottleship/bin
RUN cp tst /bottleship/bin
WORKDIR /bottleship/bin
ENTRYPOINT /bottleship/bin/bottleship
