#!/bin/sh

all:
	cc -c verticalsum.c
	nvcc -c verticalsum_gpu.cu
	nvcc -c transpose.cu
	nvcc verticalsum.o verticalsum_gpu.o transpose.cu -o vs -lcudpp -lcudart

clean:
	/bin/rm -f *.o
	/bin/rm -f vs
