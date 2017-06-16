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

//typedef struct cl_context


/*! Selects mango platform and then initializes the context  */
cl_context CreateContext();


#ifdef __cplusplus
}
#endif

#endif /* MANGO_H */

