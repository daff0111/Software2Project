#ifndef OPENCL_WRAP_H
#define OPENCL_WRAP_H

/*! \mainpage
 * 	This set of header files describes a wrapper from OPENCL to MANGO architecture.
 *	Altought OpencCl is a framework that offers various functionalities,
 *  this code is focused on the parallel computing interface, making the bridge between both languages.
 *  The code below focous on the following points:
 *      - Context
 *      - Buffer/Memory Objects
 *      - Programs
 *      - Kernels
 *      - Events
 *      - Command Queue
 *      - Exceptions  
 *
 */
#ifndef __cplusplus
#include <stdint.h>
#include <stdlib.h>
//#include <stddef.h>
#endif

#ifdef __cplusplus
#include "mango.h"

extern "C" {
#endif 

/* ************************************************ */


/* cl_device_type - bitfield */
#define CL_DEVICE_TYPE_DEFAULT                      (1 << 0)
#define CL_DEVICE_TYPE_CPU                          (1 << 1)
#define CL_DEVICE_TYPE_GPU                          (1 << 2)
#define CL_DEVICE_TYPE_ACCELERATOR                  (1 << 3)
#define CL_DEVICE_TYPE_CUSTOM                       (1 << 4)
#define CL_DEVICE_TYPE_ALL                          0xFFFFFFFF

/* Error Codes */
#define CL_SUCCESS                                  0
#define CL_DEVICE_NOT_FOUND                         -1	
#define CL_INVALID_PLATFORM                         -32
#define CL_INVALID_HOST_PTR                         -37

/* cl_mem_flags and cl_svm_mem_flags - bitfield */
#define CL_MEM_READ_WRITE                           (1 << 0)
#define CL_MEM_WRITE_ONLY                           (1 << 1)
#define CL_MEM_READ_ONLY                            (1 << 2)
#define CL_MEM_USE_HOST_PTR                         (1 << 3)
#define CL_MEM_ALLOC_HOST_PTR                       (1 << 4)
#define CL_MEM_COPY_HOST_PTR                        (1 << 5)

/* cl_bool */
#define CL_FALSE                                    0
#define CL_TRUE                                     1
#define CL_BLOCKING                                 CL_TRUE
#define CL_NON_BLOCKING                             CL_FALSE	


/* scalar types */
typedef int32_t         cl_int      __attribute__((aligned(4)));
typedef uint64_t        cl_ulong    __attribute__((aligned(8)));
typedef cl_ulong            cl_bitfield;
typedef cl_bitfield         cl_mem_flags;
typedef uint32_t        cl_uint     __attribute__((aligned(4)));
typedef cl_uint             cl_bool; 

/* abstract types */
typedef struct _cl_context *        cl_context;
typedef struct _cl_device_id *      cl_device_id;
typedef struct _cl_kernel *         cl_kernel;
typedef struct _cl_program *        cl_program;
typedef struct _cl_command_queue *  cl_command_queue;
typedef struct _cl_event *          cl_event;
typedef struct _cl_mem *            cl_mem;
typedef struct _cl_platform_id *    cl_platform_id;
typedef intptr_t            cl_context_properties;
typedef cl_bitfield         cl_device_type;


/* ************************************************ */ 


/*  Get Available Platform
 *	This command obtains the list of available OpenCL platforms.
 *  clGetPlatformIDs returns the number of available platforms. 
 *  The number of platforms returned can be limited with num_entries, 
 *  which can be greater than 0 and less than or equal to the number of available platforms. 
 *  We will limit the number of platforms returned by MANGO to 1.
 */ 
 cl_int clGetPlatformIDs (cl_uint num_entries, cl_platform_id * platforms, cl_uint * num_platforms);
 
 //NOT IMPLEMENTED: GetPlatformInfo
 //cl_int clGetPlatformInfo (cl_platform_id platform, cl_platform_info param_name, size_t param_value_size, void * param_value, size_t * param_value_size_ret);
 
/*  Get Available Devices
 *	Associated with each platform is a set of compute devices that an application uses to execute code. 
 *  Given a platform, a list of supported devices can be queried with the command.
 *  MANGO will return just one available Device of type CPU
 */ 
 cl_int clGetDeviceIDs (cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id *devices, cl_uint *num_devices);
 
 //NOT IMPLEMENTED: GetDeviceInfo
 //cl_int clGetDeviceInfo (cl_device_id device, cl_device_info param_name, size_t param_value_size, void * param_value, size_t * param_value_size_ret);

/*  Create a new MANGO context
 *	In OpenCL this function needs to handle which platform to use, 
 *	and in consequence the variables of these platforms. 
 *  The wrapper will then make use of cl_context being defined as an abstract data type 
 * 	and thus create a context that can be used by mango.
 *  NOTE: this implementation makes the call to the function OpenCL compliant 
 * 	but NOT INTERCHANGEABLE. 
 */ 
cl_context clCreateContextFromType(cl_context_properties *properties, cl_device_type device_type, void (*pfn_notify) (const char *errinfo, const void  *private_info, size_t  cb, void  *user_data), void  *user_data, cl_int  *errcode_ret);


//cl_command_queue clCreateCommandQueue();


/* 	Create a new MANGO program
 *	in the program flow comparison of both languages,
 * 	the create program step in OpenCL can be paired to the kernelFuncion definition in mango
 *	the orignial openCL function was clCreateProgramWithSource, but it had to be adpated
 *	due to incomatible vars. Thus, the new function in "opencCL-like format" is here presented
 * 	\returns the kernelfunction. please note that the structures tough equivalent are NOT the same 
*/
cl_program clCreateProgram(cl_context context, cl_device_id device, const char* fileName);

/* 	Create a new MANGO kernel
 * 	the create kernel step in OpenCL can be paired to the register_kernel definition in mango
 *	the orignial openCL function includes a name string to identify the kernel 
 *	to make it easier to be adapted the kernel id is made into a cl_uint instead.
*/
cl_kernel clCreateKernel(cl_program program, const cl_uint kernel_id, cl_int *errcode_ret);

/* Release the Kernel
 * done in mango by deregister_kernel
*/
//cl_int clReleaseKernel(cl_kernel kernel);

/* 	Set Arguments of a Kernel
*/
cl_int clSetKernelArg(cl_kernel kernel, cl_uint arg_index, size_t *arg_size, const void *arg_value);
 

/* 	Create a CommandQueue translates to Mango taskGraph.
*/
cl_command_queue* clCreateCommandQueue (cl_context context, cl_command_queue_properties properties, cl_int *errcode_ret);


/* 	Create a new buffer 
 * 	in OpenCL this function actually does more than the equivalent in MANGO
 *	not only it creates the buffer as it is possible to set flags to transfer data to the device
 *	this ends up changingh a little the mango program flow, nothing to worry though.
 *	\returns the address of the created buffer. once again the structures were adapted and should
 *  never be treated as equals.
*/
cl_mem clCreateBuffer(cl_kernel kernel, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret);

/*  Copy data from host to device
 *	Simple function that will get elements into host memory and copy them the specified buffer
 *	in the device. 
 *  If some event is especified int the function call mango will wait for the completion
 *  The blocking write and event functions are disabled
*/

cl_int clEnqueueWriteBuffer (cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset,	size_t cb, const void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event);

/* Read data from device
 * Get elements stored in the device buffer to the host mcontext 
 * Bocking read and creating new event functionalities are disabled
 *  If some event is especified int the function call mango will wait for the completion	
*/ 

cl_int clEnqueueReadBuffer (cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, void *ptr, cl_uint num_events_in_wait_list,	const cl_event *event_wait_list, cl_event *event);



// mango does not have an equivalent, can be made a mock if needed
//cl_int clReleaseProgram(cl_program program);

cl_int clReleaseMemObject(cl_mem memobj);

cl_int clReleaseContext(cl_context context);


#ifdef __cplusplus
}
#endif

#endif /* OPENCL_WRAP_H */

