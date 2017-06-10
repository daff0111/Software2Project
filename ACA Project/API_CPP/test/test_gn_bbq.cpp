/*
 * This application demonstrates minimal use of the C++ API.
 */
#include "mango.h"
#include "debug.h"
#include <stdlib.h>
#define KID 1
#define BIN 1
#define BOUT 2

using namespace std;

int main(int argc, char **argv){
	if (argc<2) {
		fprintf(stderr, "Not enough arguments\n");
		return 1;
	}
	// Initialization
	auto mango_rt = mango::BBQContext();
	auto k = mango::KernelFunction();
	k.load("./test_kernel_bbq", mango::UnitType::GN, mango::FileType::BINARY);

	// Registration of task graph
	auto k1 = mango_rt.register_kernel(KID, k, {BIN}, {BOUT});
	auto b1 = mango_rt.register_buffer<mango::Buffer>(BIN, sizeof(char)*4, {}, {KID});
	auto b2 = mango_rt.register_buffer<mango::Buffer>(BOUT, sizeof(char)*4, {KID}, {});
	auto tg = mango::TaskGraph({ k1 }, { b1, b2 });
	
	// Resource Allocation
	if (mango_rt.resource_allocation(tg)!=mango::ExitCode::SUCCESS)
		return -1;
	
	mango_rt.print(__FUNCTION__,__LINE__);
	
	// Execution setup
	auto arg1 = mango::BufferArg( b1 );
	auto arg2 = mango::BufferArg( b2 );
	auto arg3 = mango::EventArg( b2->event );
	
	auto args = mango::KernelArguments({ &arg1, &arg2, &arg3 }, k1);

	// Data transfer and kernel execution
	b1->write(argv[1]);
	auto e2=mango_rt.start_kernel(k1, args);
	b2->event->wait();
	e2->wait();

	char a[10]={0};
	b2->read(a);
	printf("Out: %s\n", a);
	// Deallocation and teardown
	mango_rt.resource_deallocation(tg);

	return 0;
}
