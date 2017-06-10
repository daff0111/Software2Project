#ifndef KERNEL_RUNNER_H
#define KERNEL_RUNNER_H
#include "mango.h"

class KernelRunner {
	private:
		mango::Context *mango_rt;
		mango::KernelArguments *argsKSCALE;
		mango::KernelArguments *argsKCOPY;
		mango::KernelArguments *argsKSMOOTH;
		mango::TaskGraph *tg;
		enum { KSCALE=1, KCOPY, KSMOOTH };
		enum { B1=1, B2, B3 };

	public:
		KernelRunner(int SX, int SY){
			// Initialization
			mango_rt = new mango::Context();
			auto kf_scale = new mango::KernelFunction();
			kf_scale->load("./scale_kernel", mango::UnitType::GN,
				mango::FileType::BINARY);
			auto kf_copy = new mango::KernelFunction();
			kf_copy->load("./copy_kernel", mango::UnitType::GN,
				mango::FileType::BINARY);
			auto kf_smooth = new mango::KernelFunction();
			kf_smooth->load("./smooth_kernel", mango::UnitType::GN,
				mango::FileType::BINARY);

			// Registration of task graph
			auto kscale  = mango_rt->register_kernel(KSCALE, *kf_scale, {B1}, {B2});
			auto kcopy   = mango_rt->register_kernel(KCOPY, *kf_copy, {B2}, {B3});
			auto ksmooth = mango_rt->register_kernel(KSMOOTH, *kf_smooth, {B2,B3},
				{B3});

			auto b1 = mango_rt->register_buffer<mango::Buffer>(B1,
				 SX*SY*3*sizeof(Byte), {}, {KSCALE});
			auto b2 = mango_rt->register_buffer<mango::Buffer>(B2,
				 SX*2*SY*2*3*sizeof(Byte), {KSCALE}, {KCOPY, KSMOOTH});
			auto b3 = mango_rt->register_buffer<mango::Buffer>(B3,
				 SX*2*SY*2*3*sizeof(Byte), {KCOPY, KSMOOTH}, {KSMOOTH});

			tg = new mango::TaskGraph({ kscale, kcopy, ksmooth }, { b1, b2, b3 });

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

			argsKSCALE = new mango::KernelArguments(
				{ &argB2, &argB1, &argSX, &argSY }, kscale);
			argsKCOPY = new mango::KernelArguments(
				{ &argB3, &argB2, &argSX2, &argSY2 }, kcopy);
			argsKSMOOTH = new mango::KernelArguments(
				{ &argB3, &argB2, &argSX2, &argSY2 }, ksmooth);
		}

		~KernelRunner() {
			// Deallocation and teardown
			mango_rt->resource_deallocation(*tg);
		}

		void run_kernel(Byte *out, Byte *in) {
			auto b1 = mango_rt->buffers[B1];
			auto b3 = mango_rt->buffers[B3];
			auto kscale  = mango_rt->kernels[KSCALE];
			auto kcopy   = mango_rt->kernels[KCOPY];
			auto ksmooth = mango_rt->kernels[KSMOOTH];

			// Data transfer and kernel execution
			b1->write(in);

			auto e1=mango_rt->start_kernel(kscale, *argsKSCALE);
			e1->wait();

			auto e2=mango_rt->start_kernel(kcopy, *argsKCOPY);
			e2->wait();

			auto e3=mango_rt->start_kernel(ksmooth, *argsKSMOOTH);
			e3->wait();

			b3->read(out);
		}
};

#endif
