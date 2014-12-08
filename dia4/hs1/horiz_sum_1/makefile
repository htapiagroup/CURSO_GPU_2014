#!/bin/sh

all:
	cc -c horizontalsum.c
	nvcc -c horizontalsum_gpu.cu
	nvcc horizontalsum.o horizontalsum_gpu.o -o vs -lcudpp -lcudart
clean:
	rm -f *.o
	rm -f vs
