#include <cuda.h>
#include <stdio.h>
#include <cudpp.h>
#include <unistd.h>

size_t pitch;


extern "C" double* matrix_alloc_gpu (int size_x, int size_y) {
  void *p;
  cudaMallocPitch (&p, &pitch, sizeof(double)*size_x, size_y);
  if (p == NULL) {
    fprintf (stderr, "I cannot reserve device memory for the matrix.\n");
    exit (EXIT_FAILURE);
  }
  return (double *)p;
}

extern "C" void H2D (double *m_gpu, double *m_cpu, int size_x, int size_y) {
  cudaMemcpy2D (m_gpu, pitch, m_cpu, size_x*sizeof(double),\
		size_x*sizeof(double), size_y,cudaMemcpyHostToDevice);
}

extern "C" void D2H (double *m_cpu, double *m_gpu, int size_x, int size_y) {
  //AQUI: ponga la instruccion correcta de copia de la memoria del
  //device a la memoria del host
}


extern "C" void main_gpu (double *d_odata, double *d_idata, int size_x, int size_y) {
  CUDPPHandle theCudpp;
  CUDPPConfiguration config;
  CUDPPHandle scanplan = 0;
  CUDPPResult res;
  
  cudppCreate (&theCudpp);
  config.op = CUDPP_ADD;
  config.datatype = CUDPP_DOUBLE;
  config.algorithm = CUDPP_SCAN;
  //AQUI: Fijese en el tipo de scan efectuado en el host, y complete las opciones (config.options)
  
  //AQUI: fabrique el plan de la reduccion. Cuidado con el valor de 'rowPitch'
  
  if (CUDPP_SUCCESS != res) {
    printf("Error creating CUDPPPlan\n");
    exit(-1);
  }
  
  //AQUI: ponga la instruccion para la ejecucion del plan

  if (CUDPP_SUCCESS != res) {
    printf("Error in cudppScan()\n");
    exit(-1);
  }
}
