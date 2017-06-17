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
#include "context.h"

extern "C" {
#endif 

/* ************************************************ */

/* scalar types */
typedef signed   __int32        cl_int;
typedef unsigned __int64        cl_ulong;
typedef cl_ulong            cl_bitfield;
typedef cl_bitfield         cl_mem_flags;
//typedef unsigned __int32        cl_uint;

/* abstract types */
typedef struct _cl_context *        cl_context;
typedef struct _cl_device_id *      cl_device_id;
typedef struct _cl_kernel *         cl_kernel;
typedef struct _cl_program *        cl_program;
typedef struct _cl_command_queue *  cl_command_queue;
typedef struct _cl_event *          cl_event;
typedef struct _cl_mem *            cl_mem;
//typedef struct _cl_platform_id *    cl_platform_id;


//typedef intptr_t            cl_context_properties;

/* ************************************************ */ 


/*! Selects mango platform and then initializes the context  */
cl_context CreateContext();

cl_program CreateProgram(cl_context context, cl_device_id device, 
                         const char* fileName);

cl_kernel clCreateKernel(cl_program program, const char *kernel_name, cl_int *errcode_ret);

cl_mem clCreateBuffer(cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret
	);

clReleaseProgram();
clReleaseMemObject();


#ifdef __cplusplus
}
#endif

#endif /* MANGO_H */

