FROM gcc:11.3.0

COPY . /ntua_assignment
VOLUME ./img

RUN apt-get update
RUN apt-get install -y cmake python-is-python3 python3-pip

RUN pip install matplotlib numpy

WORKDIR /ntua_assignment/build
RUN cmake ..
RUN make
CMD ["./ransac"]
