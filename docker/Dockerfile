FROM buildpack-deps:latest

RUN apt update
RUN apt install -y cmake ninja-build clang-16

RUN cd /usr/src; git clone https://github.com/m-kurtenacker/anydsl --branch cmake-based-setup

COPY config.sh /usr/src/anydsl/
COPY patch-and-make.sh /usr/src/anydsl/

RUN cd /usr/src/anydsl; ./setup.sh

ENTRYPOINT bash --init-file /usr/src/anydsl/project.sh
