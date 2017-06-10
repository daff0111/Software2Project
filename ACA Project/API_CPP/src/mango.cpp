#include "mango.h"
#include <stdarg.h>

using namespace std;

static mango::Context *cxt;

extern "C" {

mango_exit_t mango_init(){
	cxt = new mango::Context;
	return SUCCESS;
}

mango_exit_t mango_release(){
	delete cxt;
	return SUCCESS;
}

kernelfunction *mango_kernelfunction_init(){
	mango::KernelFunction *k = new mango::KernelFunction();
	return (kernelfunction*)k;
}

mango_exit_t mango_load_kernel(
	char *kname, kernelfunction *kernel,
	mango_unit_type_t unit, filetype t){
	((mango::KernelFunction *)kernel)->load(kname, (mango::UnitType)unit,
		 (mango::FileType)t);
	return SUCCESS;
}

mango_kernel_t mango_register_kernel(uint32_t kernel_id, 
	kernelfunction *kernel,  int nbuffers_in, int nbuffers_out, ...){
	mango::KernelFunction *t = (mango::KernelFunction *)kernel;
	std::vector<uint32_t> in;
	std::vector<uint32_t> out;
	va_list list;
	va_start(list, nbuffers_out);
	for(int i=0; i<nbuffers_in; i++)
		in.push_back(va_arg(list, mango_buffer_t));
	for(int i=0; i<nbuffers_out; i++)
		out.push_back(va_arg(list, mango_buffer_t));
	va_end(list);
	cxt->register_kernel(kernel_id, *t, in, out);
	return kernel_id;
}

mango_buffer_t mango_register_memory(uint32_t buffer_id, size_t size, mango_buffer_type_t mode, int nkernels_in, int nkernels_out, ...){
	std::vector<uint32_t> in;
	std::vector<uint32_t> out;
	va_list list;
	va_start(list, nkernels_out);
	for(int i=0; i<nkernels_in; i++)
		in.push_back(va_arg(list, mango_kernel_t));
	for(int i=0; i<nkernels_out; i++)
		out.push_back(va_arg(list, mango_kernel_t));
	va_end(list);
	if (mode == FIFO)	
		cxt->register_buffer<mango::FIFOBuffer>(buffer_id, size, in, out);
	else
		cxt->register_buffer<mango::Buffer>(buffer_id, size, in, out);
	return buffer_id;	
}

void mango_deregister_memory(mango_buffer_t mem){
	cxt->deregister_buffer(mem);
}

void mango_resize_memory(mango_buffer_t mem, size_t size){
	cxt->buffers[mem]->resize(size);
}

mango_event_t mango_get_buffer_event(mango_buffer_t buffer){
	return cxt->buffers[buffer]->event->id;
}

mango_task_graph_t *mango_task_graph_create(int k, int b, int e, ...){
	va_list list;
	va_start(list, e);
	mango::TaskGraph *tg = new mango::TaskGraph();
	for(int i=0; i<k; i++)
		*tg+=cxt->kernels[va_arg(list, mango_kernel_t)];
	for(int i=0; i<b; i++)
		*tg+=cxt->buffers[va_arg(list, mango_buffer_t)];
	for(int i=0; i<e; i++)
		*tg+=cxt->events[va_arg(list, mango_event_t)];
	va_end(list);
	for(auto &e : cxt->events)
		*tg+=e.second;
	return (TaskGraph *)tg;
}

void mango_task_graph_destroy_all(mango_task_graph_t *task_graph){
	delete (mango::TaskGraph *)task_graph;
}


mango_exit_t mango_resource_allocation(mango_task_graph_t *tg){
	return (mango_exit_t) cxt->resource_allocation(*((mango::TaskGraph *)tg));
}

void mango_resource_deallocation(mango_task_graph_t *tg){
	cxt->resource_deallocation(*((mango::TaskGraph *)tg));
}

mango_event_t mango_write(void *GN_buffer, mango_buffer_t HN_buffer,
	mango_communication_mode_t mode, size_t global_size){
	std::shared_ptr<mango::Event> e;
	auto b=cxt->buffers[HN_buffer];
	if (mode == BURST) {
		auto f = std::static_pointer_cast<mango::FIFOBuffer>(b);
		e = f->write(GN_buffer,global_size);
	} else
		e=cxt->buffers[HN_buffer]->write(GN_buffer);
	return e->id;
}

mango_event_t mango_read(void *GN_buffer, mango_buffer_t HN_buffer,
	mango_communication_mode_t mode, size_t global_size){
	std::shared_ptr<mango::Event> e;
	auto b = cxt->buffers[HN_buffer];
	if (mode == BURST){
		auto f =
			std::static_pointer_cast<mango::FIFOBuffer>(cxt->buffers[HN_buffer]);
		e=f->read(GN_buffer,global_size);
	}	else
		e=b->read(GN_buffer);
	return e->id;
	
}

mango_arg_t *mango_arg(void *value, size_t size, mango_buffer_type_t t){
	mango::Arg *a=NULL;
	switch (t) {
		case SCALAR : 
			switch (size) { 
				case sizeof(uint64_t) : 
					a = new mango::ScalarArg<uint64_t>((uint64_t)value); 
					break;
				case sizeof(uint32_t) : 
					a = new mango::ScalarArg<uint32_t>((uint32_t)value); 
					break;
				case sizeof(uint16_t) : 
					a = new mango::ScalarArg<uint16_t>((uint16_t)value); 
					break;
				case sizeof(uint8_t) : 
					a = new mango::ScalarArg<uint8_t>((uint8_t)value); 
					break;
			}
			break;
		case EVENT : 
			a = new mango::EventArg(cxt->events[(uint32_t)value]); 
			break;
		case BUFFER : 
		case FIFO : 
			a = new mango::BufferArg(cxt->buffers[(uint32_t)value]); 
			break;
		default : break;
	}
	return (mango_arg_t *) a;
}

mango_args_t *mango_set_args(mango_kernel_t kernel, int argc, ...){
	std::vector<mango::Arg *> arguments;
	va_list list;
	va_start(list, argc);
	for(int i=0; i<argc; i++)
		arguments.push_back((mango::Arg *)va_arg(list, mango_arg_t *));
	va_end(list);
	auto k = cxt->kernels[kernel];
	return (mango_args_t *) new mango::KernelArguments(arguments, k);
}

mango_event_t mango_start_kernel(mango_kernel_t kernel, mango_args_t *args, mango_event_t event){
	auto k =cxt->kernels[kernel];
	auto e =cxt->events[event];
	return cxt->start_kernel(k,*((mango::KernelArguments *)args),e)->id;
}

void mango_wait(mango_event_t e){
	cxt->events[e]->wait();
}


void mango_write_synchronization(mango_event_t event, uint32_t value){
	cxt->events[event]->write(value);
};

uint32_t mango_read_synchronization(mango_event_t event){
	return cxt->events[event]->read();
};


}
