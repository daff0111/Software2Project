#ifndef MM_H
#define MM_H
/*! \file mm.h
 * \brief Memory manager stub for GN implementation
 */ 

#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "task_graph.h"

/*! Redefine externally BASETLB if the current value conflicts with your system setup */
#ifndef BASETLB
#define BASETLB 0x1000000
#endif

namespace mango {

/*! \brief Stub of a Memory Manager
 * Currently, it only manages in a very rough way the virtual addresses
 */
class MM {
	uint64_t size;
	uint64_t last_address;
	uint8_t *memory;
	uint64_t baseTLB;
	int tf;
	
	public :
		MM(uint64_t _size=1048576) {
			baseTLB=BASETLB; // Should be one per kernel
			size=_size;
			last_address=1024;
			#ifndef UPV_HN_ENABLED
			int tf=open("device_memory.dat",O_RDWR|O_CREAT,S_IRWXU);
			if (!tf) std::cerr << "Memory mapping file not available" << std::endl;
			lseek(tf, size, SEEK_SET);
			write(tf, "", 1); /* Set file size to size */
			memory = (uint8_t*) mmap (NULL,size, 
					PROT_READ|PROT_WRITE,MAP_SHARED,tf,0);
			std::cerr << "Memory mapped at " << (uint64_t)memory << std::endl;
			if (memory == MAP_FAILED)
				std::cerr << "Memory mapping failed" << std::endl;
			std::memset(memory, 0, size);
			#endif /* UPV_HN_ENABLED */
		}

		~MM() {
				#ifndef UPV_HN_ENABLED
				munmap(memory, size);
				close(tf);
				#endif /* UPV_HN_ENABLED */
		}

		ExitCode allocate_events(
			TaskGraph &tg, 
			std::map<uint32_t, std::shared_ptr<Event>> &events) {
			std::cerr << "Allocate events" << std::endl;
			for(auto& b : tg.events){
				events[b->id]->gn_addr=(uint32_t *) 
					& memory[b->phy_addr];
			}
			return ExitCode::SUCCESS;
		}

		ExitCode set_vaddr_events(
			TaskGraph &tg, 
			std::map<uint32_t, std::shared_ptr<Event>> &events,
			std::map<uint32_t, std::shared_ptr<Kernel>> &kernels) {
			std::cerr << "Allocate events" << std::endl;
			for(auto& b : tg.events){
				for(auto &k : events[b->id]->in)
					if (k!=0) kernels[k]->tlb_events[b->id]=baseTLB+(b->phy_addr);
				for(auto &k : events[b->id]->out)
					if (k!=0) kernels[k]->tlb_events[b->id]=baseTLB+(b->phy_addr);
			}
			for(auto& k : tg.kernels)
				for(auto &e : k->task_events)
					k->tlb_events[e->id]=baseTLB+(e->phy_addr);
			return ExitCode::SUCCESS;
		}
		
		ExitCode allocate_buffers(
			TaskGraph &tg, 
			std::map<uint32_t, std::shared_ptr<Buffer>> &buffers) {
			for(auto& b : tg.buffers){
				if (last_address+buffers[b->id]->size>size)
					return ExitCode::ERR_OUT_OF_MEMORY;
				buffers[b->id]->phy_addr=last_address;
				#ifdef UPV_HN_ENABLED
				buffers[b->id]->gn_addr=last_address;
				#else 
				buffers[b->id]->gn_addr=&memory[last_address];
				#endif /* UPV_HN_ENABLED */
				std::cerr << "Assigning address " << last_address << " to buffer " << b->id << std::endl;
				last_address+=buffers[b->id]->size;
			}
			return ExitCode::SUCCESS;
		}
		
		ExitCode set_vaddr_buffers(
			TaskGraph &tg, 
			std::map<uint32_t, std::shared_ptr<Buffer>> &buffers,
			std::map<uint32_t, std::shared_ptr<Kernel>> &kernels) {
			for(auto& b : tg.buffers){
				/* \todo Fix the temporary hack for TLB */
				for(auto &k : buffers[b->id]->in) {
					if (k==0) continue;
					kernels[k]->tlb[b->id]=baseTLB+buffers[b->id]->phy_addr;
					std::cerr << "Mapping buffer" << b->id << " to kernel " << k << " at " << baseTLB+buffers[b->id]->phy_addr << std::endl;
				}
				for(auto &k : buffers[b->id]->out) {
					if (k==0) continue;
					kernels[k]->tlb[b->id]=baseTLB+buffers[b->id]->phy_addr;
					std::cerr << "Mapping buffer" << b->id << " to kernel " << k << " at " << baseTLB+buffers[b->id]->phy_addr << std::endl;
				}
				/* End TLB hack */
			}
			return ExitCode::SUCCESS;
		}
		
		ExitCode deallocate_buffers(
			TaskGraph &tg, std::map<uint32_t,
			std::shared_ptr<Buffer>> &buffers) {
			std::cerr << "Sorry, no memory deallocation at the moment" << std::endl;
			return ExitCode::ERR_FEATURE_NOT_IMPLEMENTED;
		}
};

}
#endif /* MM */
