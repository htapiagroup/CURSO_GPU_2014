all : addition.o addongpu.o
	nvcc addition.o addongpu.o 

addition.o : addition.c
	cc -c addition.c

addongpu.o : addongpu.cu
	nvcc -c addongpu.cu
