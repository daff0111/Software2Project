#ifndef KERNEL_RUNNER_H
#define KERNEL_RUNNER_H
#include "mango.h"

class KernelRunner {
	private:
		mango::BBQContext *mango_rt;
		mango::KernelArguments *argsKSCALE;
		mango::KernelArguments *argsKSMOOTH;
		mango::TaskGraph *tg;
		enum { HOST=0, KSCALE=1, KSMOOTH };
		enum { B1=1, B2, B3, B4 };

	public:
		KernelRunner(int SX, int SY){
			// Initialization
			mango_rt = new mango::BBQContext();
			auto kf_scale = new mango::KernelFunction();
			kf_scale->load("./scale_kernel", mango::UnitType::GN,
				mango::FileType::BINARY);
			auto kf_smooth = new mango::KernelFunction();
			kf_smooth->load("./smooth_kernel", mango::UnitType::GN,
				mango::FileType::BINARY);

			// Registration of task graph
			auto kscale  = mango_rt->register_kernel(KSCALE, *kf_scale, {B1}, {B2});
			auto ksmooth = mango_rt->register_kernel(KSMOOTH, *kf_smooth, {B2}, {B3});

			auto b1 = mango_rt->register_buffer<mango::Buffer>(B1,
				 SX*SY*3*sizeof(Byte), {HOST}, {KSCALE});
			auto b2 = mango_rt->register_buffer<mango::Buffer>(B2,
				 SX*2*SY*2*3*sizeof(Byte), {KSCALE}, {KSMOOTH});
			auto b3 = mango_rt->register_buffer<mango::Buffer>(B3,
				 SX*2*SY*2*3*sizeof(Byte), {KSMOOTH}, {HOST});

			tg = new mango::TaskGraph({ kscale, ksmooth }, { b1, b2, b3 });

			// Resource Allocation
			mango_rt->resource_allocation(*tg);
			
			// Execution setup
			auto argB1 = mango::BufferArg( b1 );
			auto argB2 = mango::BufferArg( b2 );
			auto argB3 = mango::BufferArg( b3 );
			auto argSX = mango::ScalarArg<int>( SX );
			auto argSY = mango::ScalarArg<int>( SY );
			auto argSX2 = mango::ScalarArg<int>( SX*2 );
			auto argSY2 = mango::ScalarArg<int>( SY*2 );
			auto argE1 = mango::EventArg( b1->event );
			auto argE2 = mango::EventArg( b2->event );
			auto argE3 = mango::EventArg( b3->event );

			argsKSCALE = new mango::KernelArguments(
				{ &argB2, &argB1, &argSX, &argSY, &argE1, &argE2 }, kscale);
			argsKSMOOTH = new mango::KernelArguments(
				{ &argB3, &argB2, &argSX2, &argSY2, &argE2, &argE3 }, ksmooth);
		}

		~KernelRunner() {
			// Deallocation and teardown
			mango_rt->resource_deallocation(*tg);
		}

		void run_kernel(Byte *out, Byte *in) {
			auto b1 = mango_rt->buffers[B1];
			auto b3 = mango_rt->buffers[B3];
			auto kscale  = mango_rt->kernels[KSCALE];
			auto ksmooth = mango_rt->kernels[KSMOOTH];

			// Data transfer and kernel execution
			b1->write(in);
			b1->event->write(1);

			auto e1=mango_rt->start_kernel(kscale, *argsKSCALE);
			auto e2=mango_rt->start_kernel(ksmooth, *argsKSMOOTH);

			b3->event->wait();

			e1->wait();
			e2->wait();
			b3->read(out);
		}
};

#endif
