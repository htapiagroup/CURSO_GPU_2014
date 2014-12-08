# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions
export PATH=$PATH:/share/apps/cuda/bin
export CUDA=/share/apps/cuda
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/share/apps/cuda/lib64


export PATH=/share/apps/mvapich2/bin:$PATH
export LD_LIBRARY_PATH=/share/apps/mvapich2/lib:$LD_LIBRARY_PATH
export MV2_USE_CUDA=1

export PATH=/share/apps/openmpi/bin:$PATH
export LD_LIBRARY_PATH=/share/apps/openmpi/lib:$LD_LIBRARY_PATH
