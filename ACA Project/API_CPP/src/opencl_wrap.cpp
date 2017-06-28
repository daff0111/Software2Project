#include "opencl_wrap.h"
#include <stdarg.h>

using namespace std;

extern "C"{

// global var in order to allocate buffers 
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


// in the current implementation of mango it is not possible to not adapt!
// the cl_arg does not exist, it was created only for "compliant call" reasons
cl_arg *clSetKernelArg(cl_kernel kernel, cl_uint arg_index, size_t *arg_size, const void *arg_value){
	
	mango_arg_t *arg = NULL;

		if (arg_size == sizeof(uint32_t)){
			arg = mango_arg( arg_value, arg_size, SCALAR );	
		} else if (arg_size == sizeof(cl_mem)){
			arg = mango_arg( arg_value, arg_size, BUFFER );
		}

	return (cl_arg *) arg;
}

/*
cl_command_queue* clCreateCommandQueueAdapted (cl_context context, cl_command_queue_properties properties, cl_int *errcode_ret, int k, int b, int e, ...){
	
	va_list list;
	va_start(list, e);
	mango::TaskGraph *tg = new mango::TaskGraph();
	mango_task_graph_t *tg2;
	for(int i=0; i<k; i++){
		*tg2 = mango_task_graph_create(1, 0, 0, va_arg(list, mango_kernel_t));
		*tg += tg2.kernels[0];
	}
	for(int i=0; i<b; i++){
		*tg2 = mango_task_graph_create(0, 1, 0, va_arg(list, mango_kernel_t));
		*tg += tg2.buffers[0];
	}
	for(int i=0; i<e; i++){
		*tg2 = mango_task_graph_create(0, 0, 1, va_arg(list, mango_kernel_t));
		*tg += tg2.events[0];
	}
	va_end(list);

	mango_resource_allocation((TaskGraph *)tg);

	return (cl_command_queue*) tg;
}
*/

/* cl_mem is used as mango_buffer */ 
// ADAPTED!!!! FIRST ARGUMENT WAS CHANGED
cl_mem clCreateBuffer(cl_kernel k, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret){

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

  	b = mango_register_memory(buffer_id, size, BUFFER, (flags & CL_MEM_WRITE_ONLY  || flags & CL_MEM_READ_WRITE), (flags & CL_MEM_READ_ONLY || flags & CL_MEM_READ_WRITE), k); 	
  	buffer_id++;

  	if (flags & CL_MEM_USE_HOST_PTR || flags & CL_MEM_COPY_HOST_PTR)
    	mango_write(host_ptr, b, DIRECT, 0);
exit:
	if (errcode_ret)
		*errcode_ret = err;
	return (cl_mem ) b;
error:
  	b = 0;
  	goto exit;

}

// EVENT VIEW!!! 
cl_int clEnqueueWriteBuffer (cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset,	size_t cb, const void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event){

	if(num_events_in_wait_list == 1){
		mango_wait(event_wait_list);
	} else if(num_events_in_wait_list > 1){
		for(int i=0; i< num_events_in_wait_list; i++)
			mango_wait((mango_event_t)event_wait_list[i]);
	}

	// mango_event_t is the format 
	if(event != NULL)
		event = mango_write(ptr, (mango_buffer_t) buffer, DIRECT, 0);
	
	else
		mango_write(ptr, (mango_buffer_t) buffer, DIRECT, 0);
			
	return CL_SUCCESS;
}

	// EVENT!!!!
cl_int clEnqueueReadBuffer (cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, void *ptr, cl_uint num_events_in_wait_list,	const cl_event *event_wait_list, cl_event *event){

	
	if(num_events_in_wait_list == 1){
		mango_wait((mango_event_t) event_wait_list);
	} else if(num_events_in_wait_list > 1){
		for(int i=0; i< num_events_in_wait_list; i++)
			mango_wait((mango_event_t)event_wait_list[i]);
	}

	if(event != NULL)
		event = mango_read(ptr, (mango_buffer_t) buffer, DIRECT, 0);
	
	else
		mango_read(ptr, (mango_buffer_t) buffer, DIRECT, 0);
	
	return CL_SUCCESS;

}

cl_int clEnqueueNDRangeKernel (cl_args *args, cl_kernel kernel, cl_uint work_dim, const size_t *global_work_offset, const size_t *global_work_size, const size_t *local_work_size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event){

	if(num_events_in_wait_list == 1){
		mango_wait((mango_event_t) event_wait_list);
	} else if(num_events_in_wait_list > 1){
		for(int i=0; i< num_events_in_wait_list; i++)
			mango_wait((mango_event_t)event_wait_list[i]);
	}

	if(event != NULL)
		event = mango_start_kernel(kernel, (mango_args_t *) args, NULL);
	
	else
		mango_start_kernel(kernel, (mango_args_t *) args, NULL);
	
	return CL_SUCCESS;
}


cl_int clReleaseMemObject(cl_mem memobj){

	mango_deregister_memory( (mango_buffer_t) memobj);
	return CL_SUCCESS;
}

cl_int clReleaseContext(cl_context context){
	mango_release();
	return CL_SUCCESS;
}

// other releases could be implemented as dummys for now. mango does not provide support.

}