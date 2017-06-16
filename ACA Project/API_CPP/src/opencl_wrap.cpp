#include "OpenCL_wrap.h"

using namespace std;

static mango::Context *cxt;

extern "C"{

cl_context CreateContext(){

	cl_context *context;

	cxt = new mango::Context;
	
	context = malloc (sizeof(cl_context));

	context = cxt. 


	return ;

}

}