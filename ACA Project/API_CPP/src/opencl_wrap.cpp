#include "opencl_wrap.h"
#include "mango.h"
#include <stdarg.h>

using namespace std;

static mango::Context *ctx;

extern "C"{

//int read_buffer = 0;
//int write_buffer = 0;

//PLATFORM
cl_int clGetPlatformIDs (cl_uint num_entries, cl_platform_id * platforms, cl_uint * num_platforms){
	cl_int err = CL_SUCCESS;
	
	//Always return one available platform
	*num_platforms = 1;
	
	//num_entries will not affect the platform availability
	
	//platforms pointer can be null
	if(platforms != NULL)
	{
		*platforms = NULL; //mango platform?
	}
	
	return err;
}

//DEVICE 
cl_int clGetDeviceIDs (cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id *devices, cl_uint *num_devices){
	cl_int err = CL_SUCCESS;
	
	if(platform == NULL) //check platform?
	{
		//num_entries will not affect the number of devices	
			
		switch(device_type)
		{
		case CL_DEVICE_TYPE_CPU:
		case CL_DEVICE_TYPE_DEFAULT:
		case CL_DEVICE_TYPE_ALL:
		{
			//Always return one available CPU device
			*num_devices = 1;
			if(devices != NULL)
			{
				*devices = NULL; //fake device?
			}
		}
		case CL_DEVICE_TYPE_GPU:
		case CL_DEVICE_TYPE_ACCELERATOR:
		default:
		{
			*num_devices = 0;
			err = CL_DEVICE_NOT_FOUND;
		}
	}
	else
	{
		err = CL_INVALID_PLATFORM;
	}
	return err;
}

// CL_DEVICE_TYPE_CPU 
cl_context clCreateContextFromType(cl_context_properties *properties, cl_device_type device_type, void (*pfn_notify) (const char *errinfo, const void  *private_info, size_t  cb, void  *user_data), void  *user_data, cl_int  *errcode_ret){

	cl_context ctx2 = NULL;
	cl_int err = CL_SUCCESS;
	
	// Unsupported callback
	if(pfn_notify != NULL || user_data != NULL){
		goto error;
	}

	ctx = new mango::Context; //mango_init();??
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

//KERNEL
cl_kernel clCreateKernel(cl_program program, const cl_uint kernel_id, cl_int *errcode_ret){
	errcode_ret = CL_SUCCESS;
	mango_kernel_t kernel = mango_register_kernel(kernel_id, (kernelfunction) program, 0, 0);  	
	return (cl_kernel) kernel;
}

cl_int clReleaseKernel(cl_kernel kernel){
	cl_int err = CL_SUCCESS;
	mango_deregister_kernel((mango_kernel_t) kernel);
	return err;
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