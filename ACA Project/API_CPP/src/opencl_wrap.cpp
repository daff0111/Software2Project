#include "OpenCL_wrap.h"

using namespace std;

static mango::Context *cxt;

extern "C"{

int read_buffer = 0;
int write_buffer = 0;

/* This is the only createcontext function in the OpenCL lib that suits our needs, since mango does not need to handle different platforms */ 
cl_context CreateContext(){

	// this is the mango instance
	cxt = new mango::Context;

	// it used as a dummy, as the context concept is different between both platforms
	return 1;
}

/* program here is handled as a kernel function */ 
cl_program CreateProgram(cl_context context, cl_device_id device, 
                         const char* fileName){

	// openCL return

	// Mango wrap
	kernelfunction *k = mango_kernel_function_init();
	mango_load_kernel(filename, k, GN, BINARY);
	
	// still need to try if it's compatible
  	return (cl_program) k;
}

/*
/* NOOOOOT FINISHED!!!!! 
/* cl_mem is used as mango_buffer  
cl_mem clCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret){

	cl_int err = CL_SUCESS;
	mango_buffer_t b;

	/* This flag is valid only if host_ptr is not NULL 
	if (UNLIKELY((flags & CL_MEM_COPY_HOST_PTR || flags & CL_MEM_USE_HOST_PTR) && host_ptr == NULL)) {
    	err = CL_INVALID_HOST_PTR;
   		goto error;
  	}

  	/* this mode of allocation is not available to be used with mango 
  	if (UNLIKELY(flags & CL_MEM_ALLOC_HOST_PTR)) {
    	err = CL_INVALID_HOST_PTR;
    	goto error;
  	}

  	/* CL_MEM_COPY_HOST_PTR and CL_MEM_USE_HOST_PTR are mutually exclusive. 
  	if (UNLIKELY(flags & CL_MEM_COPY_HOST_PTR && flags & CL_MEM_USE_HOST_PTR)) {
    	err = CL_INVALID_HOST_PTR;
    	goto error;
  	}

  	b = mango_register_memory(, size, BUFFER, (flags & CL_MEM_WRITE_ONLY  || flags & CL_MEM_READ_WRITE)  , (flags & CL_MEM_READ_ONLY || flags & CL_MEM_READ_WRITE), context->);


  	if (flags & CL_MEM_USE_HOST_PTR || flags & CL_MEM_COPY_HOST_PTR)
//    mem->host_ptr = data;

exit:
	if (errcode_ret)
		*errcode_ret = err;
	return (cl_mem) b;
error:
  	b = NULL;
  	goto exit;

}
*/


}