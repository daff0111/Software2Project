/*! \file
 *  \brief Kernel argument packing
 */
#ifndef KERNEL_ARGUMENTS_H
#define KERNEL_ARGUMENTS_H
#include "kernel.h"

namespace mango {

/*! \brief Kernel argument type 
 *
 * \note Arguments are typically pointers to buffers, with the appropriate size.
 * Alternately, they can be scalar values or events.
 * Subclasses are used for the three types.
 */
class Arg {
	public:
	virtual ~Arg() {};
	uint64_t value; /*!< Pointer to memory buffer (for memories), or value of the parameter */
	size_t size; /*!< Size of memory object for memories, and of the type otherwise */	
	uint32_t id; /*!< Used for Events and Buffer args */
};

template <class T> class ScalarArg: public Arg {
	public : 
		ScalarArg(T arg) { value=(uint64_t)arg; size=sizeof(T); };
		~ScalarArg(){};
};

class BufferArg: public Arg {
	friend Buffer;
	public : 
		BufferArg(std::shared_ptr<Buffer> arg) { 
			value=arg->phy_addr; 
			id=arg->id;
			size=sizeof(void *); 
		}
		~BufferArg(){};
};

class EventArg: public Arg {
	friend Event;
	public : 
		EventArg(std::shared_ptr<Event> arg) { 
			value=arg->phy_addr; 
			id=arg->id;
			size=sizeof(uint64_t); 
		}
		~EventArg(){};
};

/*! \brief A class describing the arguments passed to kernels.
 * Can be specialized to generate argument serialization for other platforms.
 */
class KernelArguments {
	std::vector<Arg> args;
	UnitType ut;
	kernelfp k;
	public : 
		KernelArguments(
				std::initializer_list<Arg *> arguments,
				std::shared_ptr<Kernel> kernel) {
			auto a=EventArg(kernel->event);
			a.value=kernel->tlb_events[kernel->event->id];
			args.push_back(a);
			for(auto e : kernel->task_events){
				auto a=EventArg(e);
				a.value=kernel->tlb_events[e->id];
				args.push_back(a);
			}
			for (Arg *a : arguments) {
				try { /* Lookup TLB for virtual address in this kernel */
					BufferArg& buffer_a = dynamic_cast< BufferArg& >( *a );
					a->value=kernel->tlb[buffer_a.id];
				} catch(std::bad_cast exp){};
				try { /* Lookup TLB for virtual address in this kernel */
					EventArg& event_a = dynamic_cast< EventArg& >( *a );
					std::cerr << "Event " << event_a.id << " " << kernel->tlb_events[event_a.id] << std::endl;
					a->value=kernel->tlb_events[event_a.id];
				} catch(std::bad_cast exp){};
				args.push_back(*a);
			}
			std::cerr << "Kernel " << kernel->id << std::endl;
			std::cerr << "Unit id " << kernel->unit->id << std::endl;
			ut=kernel->unit->arch;
			k=kernel->kernel->version[ut];
		};
		
		KernelArguments(
				std::vector<Arg *> arguments,
				std::shared_ptr<Kernel> kernel) {
			auto a=EventArg(kernel->event);
			a.value=kernel->tlb_events[kernel->event->id];
			args.push_back(a);
			for(auto e : kernel->task_events){
				auto a=EventArg(e);
				a.value=kernel->tlb_events[e->id];
				args.push_back(a);
			}
			for (Arg *a : arguments) {
				try { /* Lookup TLB for virtual address in this kernel */
					BufferArg& buffer_a = dynamic_cast< BufferArg& >( *a );
					a->value=kernel->tlb[buffer_a.id];
				} catch(std::bad_cast exp){};
				try { /* Lookup TLB for virtual address in this kernel */
					EventArg& event_a = dynamic_cast< EventArg& >( *a );
					std::cerr << "Event " << event_a.id << " " << kernel->tlb_events[event_a.id] << std::endl;
					a->value=kernel->tlb_events[event_a.id];
				} catch(std::bad_cast exp){};
				args.push_back(*a);
			}
			ut=kernel->unit->arch;
			std::cerr << "Kernel " << kernel->id << std::endl;
			std::cerr << "Unit id" << kernel->unit->id << std::endl;
			k=kernel->kernel->version[ut];
		};

		int n_args(){
			return args.size();
		};

		char **gn_args(){
			int i=0;
			char **argv=(char **)malloc(sizeof(char *)*(args.size()+2));
			memset(argv,0,sizeof(char *)*args.size()+2);
			argv[0]=strdup((char *)k);
			for (Arg &arg : args){
				char *s=0;
				//std::cerr << "Arg value = " << arg.value << std::endl;
				switch (arg.size) {
					case 8 : s=(char *)malloc(sizeof(char)*20);
						sprintf(s, "%p", (void *)arg.value);
						break;
					case 4 : s=(char *)malloc(sizeof(char)*10);
						sprintf(s, "%d", (uint32_t)((uint64_t)arg.value));
						break;
					default : 
						std::cerr << "Something wrong, arg size = " << arg.size << std::endl;
						std::cerr << "Arg value = " << argv[0] << std::endl;
						return NULL;
				}	
				argv[++i]=s;
			}
			argv[++i]=NULL;
			return argv;
		}
};

}
#endif /* KERNEL_ARGUMENTS_H */
