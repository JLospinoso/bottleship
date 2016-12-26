FROM ubuntu:16.04
RUN apt-get update && apt-get upgrade -y
RUN apt-get install git -y
WORKDIR /opt/bottleship/
RUN git clone https://github.com/google/googletest.git
RUN git clone https://github.com/google/leveldb.git
RUN apt-get install cmake g++ -y
RUN apt-get install libboost-system-dev libboost-filesystem-dev libwebsocketpp-dev -y
COPY . /opt/bottleship/
RUN mkdir build
WORKDIR /opt/bottleship/build
RUN cmake ..
RUN make
RUN mkdir ../bin
RUN cp bottleship tst ../bin
WORKDIR /opt/bottleship/bin
ENTRYPOINT bottleship
