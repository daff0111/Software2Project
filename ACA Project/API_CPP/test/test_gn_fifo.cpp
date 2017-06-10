/*
 * This application demonstrates fifo write communication from
 * host application to the kernel, using the C++ API.
 */
#include "mango.h"
#include "debug.h"
#include <stdlib.h>
#define KID 1
#define BID 1

int main(int argc, char **argv){
	if (argc<2) { 
		fprintf(stderr, "Not enough arguments\n");
		return 1;
	}
	// Initialization
	auto mango_rt = mango::Context();
	auto k = mango::KernelFunction();
	k.load("./test_kernel_fifo", mango::UnitType::GN, mango::FileType::BINARY);

	// Registration of task graph
	auto k1 = mango_rt.register_kernel(KID, k, {BID}, {BID});
	auto b1 = mango_rt.register_buffer<mango::FIFOBuffer>(BID, sizeof(char)*4, {KID}, {KID});
	auto tg = mango::TaskGraph({ k1 }, { b1 });

	// Resource Allocation
	if (mango_rt.resource_allocation(tg)!=mango::ExitCode::SUCCESS)
		return -1;

	// Execution setup
	auto arg1 = mango::BufferArg( b1 );
	auto arg2 = mango::EventArg( b1->event );

	auto args = mango::KernelArguments({ &arg1, &arg2 }, k1);

	// Data transfer and kernel execution
	auto e3=b1->write(argv[1], strlen(argv[1]));
	auto e2=mango_rt.start_kernel(k1, args);
	e3->wait();
	e2->wait();
	
	// Deallocation and teardown
	mango_rt.resource_deallocation(tg);

	return 0;
}
