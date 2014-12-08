#include <cuda.h>
#include <stdio.h>
#include <cudpp.h>

size_t pitch;


extern "C" void transpose (float *, float *, int, int);

extern "C" float* matrix_alloc_gpu (int size_x, int size_y) {
  void *p;
  cudaMallocPitch (&p, &pitch, sizeof(float)*size_x, size_y);
  if (p == NULL) {
    fprintf (stderr, "I cannot reserve host memory for the matrix.\n");
    exit (EXIT_FAILURE);
  }
  return (float *)p;
}

extern "C" void H2D (float *m_gpu, float *m_cpu, int size_x, int size_y) {
  cudaMemcpy2D (m_gpu, pitch, m_cpu, size_x*sizeof(float),\
		size_x*sizeof(float), size_y,cudaMemcpyHostToDevice);
}

extern "C" void D2H (float *m_cpu, float *m_gpu, int size_x, int size_y) {
  cudaMemcpy2D (m_cpu, size_x*sizeof(float), m_gpu, pitch, \
		size_x*sizeof(float), size_y,cudaMemcpyDeviceToHost);
}


extern "C" void main_gpu (float *d_odata, float *d_idata, int size_x, int size_y) {
  CUDPPHandle theCudpp;
  CUDPPHandle scanplan = 0;
  CUDPPResult res;
  CUDPPConfiguration config;
  float *tr_in, *tr_in_sum;
  float *tr_out;
  size_t pitch_tr_in, pitch_tr_out;

  cudppCreate (&theCudpp);
  config.op = CUDPP_ADD;
  config.datatype = CUDPP_FLOAT;
  config.algorithm = CUDPP_SCAN;
  //  config.options = AQUI: lea la parte CPU y complete las opciones

  cudaMallocPitch (&tr_in, &pitch_tr_in, size_y*sizeof(float), pitch/sizeof(float));

  //AQUI: reserve memoria pitched para las matrices tr_in_sum y tr_out


  // The matrix transp_tr_out has size pitch_tr_out/sizeof(float), pitch_tr_in/sizeof(float)

  if (pitch != pitch_tr_out) {
    fprintf (stderr, "Something went wrong...\n");
    exit (EXIT_FAILURE);
  }

  // AQUI: transponga la matrix d_idata en tr_in

  // AQUI: disenye el plan 'scanplan' para el multiscan
  
  if (CUDPP_SUCCESS != res) {
    printf("Error creating CUDPPPlan\n");
    exit(-1);
  }
  
  // Run the scan
  // AQUI: corra el plan previamente disenyado

  if (CUDPP_SUCCESS != res) {
    printf("Error in cudppScan()\n");
    exit(-1);
  }

  // AQUI: tranponga el resultado de tr_in_sum en tr_out

  // AQUI: copie tr_out en d_odata en el device
}
