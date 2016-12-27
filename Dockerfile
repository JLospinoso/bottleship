FROM ubuntu:16.04
RUN apt-get update && apt-get upgrade -y
RUN apt-get install git -y
RUN mkdir bottleship
RUN git clone https://github.com/google/googletest.git bottleship/googletest
RUN git clone https://github.com/google/leveldb.git bottleship/leveldb
RUN apt-get install cmake g++ -y
RUN apt-get install libboost-system-dev libboost-filesystem-dev libwebsocketpp-dev -y
RUN mkdir bottleship/bin
COPY . bottleship
RUN mkdir bottleship/build
WORKDIR bottleship/build
RUN cmake ..
RUN make
RUN cp bottleship tst ../bin
WORKDIR ../bin