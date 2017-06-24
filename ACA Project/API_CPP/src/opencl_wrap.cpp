#include "opencl_wrap.h"
#include <stdarg.h>

using namespace std;


extern "C"{

int buffer_id = 1;

//PLATFORM
cl_int clGetPlatformIDs (cl_uint num_entries, cl_platform_id * platforms, cl_uint * num_platforms){
	cl_int err = CL_SUCCESS;
	
	//Always return one available platform
	*num_platforms = 1;
	
	//num_entries will not affect the platform availability
	
	//platforms pointer can be null
	if(platforms != NULL)
	{
		*platforms = NULL; //mango platform? new cl_platform_id();
	}
	
	return err;
}

//DEVICE 
cl_int clGetDeviceIDs (cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id *devices, cl_uint *num_devices)
{

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
				*devices = NULL; //fake device? new cl_device_id();
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
	}
	else
	{
		err = CL_INVALID_PLATFORM;
	}
	return err;
}

// CL_DEVICE_TYPE_CPU 
cl_context clCreateContextFromType (cl_context_properties *properties, cl_device_type device_type, void (*pfn_notify) (const char *errinfo, const void  *private_info, size_t  cb, void  *user_data), void  *user_data, cl_int  *errcode_ret){

	cl_context ctx2 = NULL;
	cl_int err = CL_SUCCESS;
	
	// Unsupported callback
	if(pfn_notify != NULL || user_data != NULL){
		goto error;
	}

	mango_init();
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
	mango_kernel_t kernel = mango_register_kernel(kernel_id, (kernelfunction *) program, 0, 0);  	
	return (cl_kernel) kernel;
}

/*cl_int clReleaseKernel(cl_kernel kernel){
	cl_int err = CL_SUCCESS;

	mango_deregister_kernel((mango_kernel_t) kernel);
	return err;
}*/

cl_int clSetKernelArg(cl_kernel kernel, cl_uint arg_index, size_t *arg_size, const void *arg_value){
	cl_int err = CL_SUCCESS;
	mango_arg_t *arg = NULL
	switch (size) { 
		case sizeof(uint64_t) :
		case sizeof(uint32_t) :
		case sizeof(uint16_t) : 
		case sizeof(uint8_t) :
			mango_arg_t *arg = mango_arg( (void *)arg_value, arg_size, SCALAR );
			break;
		case sizeof(cl_mem) :
			mango_arg_t *arg = mango_arg( (void *)arg_value, arg_size, BUFFER );
			break;
		default : break;
	}
	//Still need to register it in the Kernel
	return err;
}


cl_command_queue* clCreateCommandQueue (cl_context context, cl_command_queue_properties properties, cl_int *errcode_ret){
	errcode_ret = CL_SUCCESS;
	mango_task_graph_t* tg = new mango::TaskGraph();
	return (cl_command_queue*) tg;
}

/* cl_mem is used as mango_buffer */ 
cl_mem clCreateBuffer(cl_kernel kernel, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret){

	cl_int err = CL_SUCCESS;
	mango_buffer_t b;

	/* This flag is valid only if host_ptr is not NULL */ 
	if ((flags & CL_MEM_COPY_HOST_PTR || flags & CL_MEM_USE_HOST_PTR) && host_ptr == NULL) {
    	err = CL_INVALID_HOST_PTR;
   		goto error;
  	}

  	/* this mode of allocation is not available to be used with mango */ 
  	if (flags & CL_MEM_ALLOC_HOST_PTR) {
    	err = CL_INVALID_HOST_PTR;
    	goto error;
  	}

  	/* CL_MEM_COPY_HOST_PTR and CL_MEM_USE_HOST_PTR are mutually exclusive. */
  	if (flags & CL_MEM_COPY_HOST_PTR && flags & CL_MEM_USE_HOST_PTR) {
    	err = CL_INVALID_HOST_PTR;
    	goto error;
  	}
  	
  	b = mango_register_memory(buffer_id, size, BUFFER, (flags & CL_MEM_WRITE_ONLY  || flags & CL_MEM_READ_WRITE), (flags & CL_MEM_READ_ONLY || flags & CL_MEM_READ_WRITE), kernel);
  	
  	//buffer_id++;

  	// not finished
 /* 	if (flags & CL_MEM_USE_HOST_PTR || flags & CL_MEM_COPY_HOST_PTR)
    	mango_write(XXXXX, YYYY, DIRECT, 0);
*/
exit:
	if (errcode_ret)
		*errcode_ret = err;
	return (cl_mem) b;
error:
  	b = NULL;
  	goto exit;

}



cl_int clEnqueueWriteBuffer ( cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset,	size_t cb, const void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event){

		 mango_write(ptr, (mango_buffer_t) buffer, DIRECT, 0);
		 return 1;
}

}