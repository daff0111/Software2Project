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

extern "C" {
#endif 

/* ************************************************ */

#define CL_DEVICE_TYPE_CPU                          (1 << 1)
#define CL_DEVICE_TYPE_GPU                          (1 << 2)

#define CL_SUCCESS                                  0


/* scalar types */
typedef int32_t         cl_int      __attribute__((aligned(4)));
typedef uint64_t        cl_ulong    __attribute__((aligned(8)));
typedef cl_ulong            cl_bitfield;
typedef cl_bitfield         cl_mem_flags;
typedef uint32_t        cl_uint     __attribute__((aligned(4)));

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

//cl_kernel clCreateKernel(cl_program program, const char *kernel_name, cl_int *errcode_ret);

/* 	Create a new buffer 
 * 	in OpenCL this function actually does more than the equivalent in MANGO
 *	not only it creates the buffer as it is possible to set flags to transfer data to the device
 *	this ends up changingh a little the mango program flow, nothing to worry though.
 *	\returns the address of the created buffer. once again the structures were adapted and should
 *  never be treated as equals.
*/
cl_mem clCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret);

//clReleaseProgram();
//clReleaseMemObject();


#ifdef __cplusplus
}
#endif

#endif /* MANGO_H */

