#include "opencl_wrap.h"
#include "mango.h"
#include <stdarg.h>

using namespace std;

static mango::Context *ctx;

extern "C"{

//int read_buffer = 0;
//int write_buffer = 0;

// CL_DEVICE_TYPE_CPU 
cl_context clCreateContextFromType(cl_context_properties *properties, cl_device_type device_type, void (*pfn_notify) (const char *errinfo, const void  *private_info, size_t  cb, void  *user_data), void  *user_data, cl_int  *errcode_ret){

	cl_context ctx2 = NULL;
	cl_int err = CL_SUCCESS;
	
	// Unsupported callback
	if(pfn_notify != NULL || user_data != NULL){
		goto error;
	}

	ctx = new mango::Context;
	ctx2 = 1;

	//need to take a look at possible errors 
exit:
	if(errcode_ret != NULL)
		*errcode_ret = err;
	return ctx2;

error:
	ctx2 = NULL;
	goto exit;
}

// ADAPTED 
cl_program clCreateProgram(cl_context context, cl_device_id device, const char* fileName){
	
	kernelfunction *k = mango_kernelfunction_init();

	//take a look at this binary arg
	mango_load_kernel(fileName, k, GN, BINARY);
	
	return (cl_program) k;
}

/* cl_mem is used as mango_buffer */ 
cl_mem clCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret){

	cl_int err = CL_SUCESS;
	mango_buffer_t b;

	/* This flag is valid only if host_ptr is not NULL */ 
	if (UNLIKELY((flags & CL_MEM_COPY_HOST_PTR || flags & CL_MEM_USE_HOST_PTR) && host_ptr == NULL)) {
    	err = CL_INVALID_HOST_PTR;
   		goto error;
  	}

  	/* this mode of allocation is not available to be used with mango */ 
  	if (UNLIKELY(flags & CL_MEM_ALLOC_HOST_PTR)) {
    	err = CL_INVALID_HOST_PTR;
    	goto error;
  	}

  	/* CL_MEM_COPY_HOST_PTR and CL_MEM_USE_HOST_PTR are mutually exclusive. */
  	if (UNLIKELY(flags & CL_MEM_COPY_HOST_PTR && flags & CL_MEM_USE_HOST_PTR)) {
    	err = CL_INVALID_HOST_PTR;
    	goto error;
  	}
  	// STIll needs to resolve how to find the buffer in question and who reads and writes 
  	b = mango_register_memory(, size, BUFFER, (flags & CL_MEM_WRITE_ONLY  || flags & CL_MEM_READ_WRITE)  , (flags & CL_MEM_READ_ONLY || flags & CL_MEM_READ_WRITE), context->);

  	// not finished
  	if (flags & CL_MEM_USE_HOST_PTR || flags & CL_MEM_COPY_HOST_PTR)
    	mango_write(XXXXX, YYYY, DIRECT, 0);

exit:
	if (errcode_ret)
		*errcode_ret = err;
	return (cl_mem) b;
error:
  	b = NULL;
  	goto exit;

}


}