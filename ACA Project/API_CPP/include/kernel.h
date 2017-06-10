/*! \file
 *  \brief Kernels
 */
#ifndef KERNEL_H
#define KERNEL_H
#include "buffer.h"
#include <map>
#include <algorithm>

namespace mango {

typedef void * (*kernelfp)(void *);

/*! \brief Kernel function 
 *
 * This is an array of function pointers to support multiple versions of the
 * kernel.
 * \note This allows one kernel implementation per type of unit. If more are 
 * desired, we need to redesign this data structure.
 */
class KernelFunction { 
	public :
  std::map<UnitType, kernelfp> version; /*!< pointer to the start of the kernel, for each unit type */
	std::map<UnitType, size_t> size; /*!< size, for each unit type */

		ExitCode load(const char *kname, UnitType unit, FileType t){
			ExitCode res;
			switch (unit){
				case UnitType::GN :	res = load_gn(kname,t); break;
				default : res = ExitCode::ERR_UNSUPPORTED_UNIT; break;
			}
			return res;
		};
		
		KernelFunction(){};
		
	private:
		ExitCode load_gn(const char *kname, FileType t){	
			switch (t) {
				case FileType::BINARY: 
					version[UnitType::GN] = (kernelfp)strdup(kname);
					size[UnitType::GN] = sizeof(char *); 
					break;
				case FileType::STRING: 
				case FileType::SOURCE: 
					return ExitCode::ERR_FEATURE_NOT_IMPLEMENTED ;
				default : 
					return ExitCode::ERR_INVALID_KERNEL_FILE ;
			}
			return ExitCode::SUCCESS;
		}

};


/*! \brief Kernel descriptor */
class Kernel {
	public:
		int id;  /*!< kernel id */
		KernelFunction *kernel;  /*!< pointer to kernel in GN memory */
		std::vector<uint32_t> in;	/*!< Read buffers */
		std::vector<uint32_t> out; /*!< Write buffers */
		std::map<uint32_t, uint64_t> tlb; /*!< map addresses of buffers */
		std::map<uint32_t, uint64_t> tlb_events; /*!< map addresses of events */
		int thread_count;  /*!< Number of cores needed for this kernel; 0 means as 	many as possible */
		std::shared_ptr<Unit> unit; /*!< The unit allocated to this kernel; NULL before mango_resource_allocation is invoked. */
		std::shared_ptr<Event> event; /*!< Synchronization event */
		std::vector<std::shared_ptr<Event>> task_events; /*!< Synch for tasks */
		Kernel(){};
		
		Kernel(int kid, KernelFunction &k, std::vector<uint32_t> in_buffers, std::vector<uint32_t> out_buffers) { 
			in=in_buffers; 
			out=out_buffers; 
			id=kid; 
			kernel=&k; 
			event=std::make_shared<Event>(kid);
			for(int i=0; i<3; i++)
				task_events.push_back(std::make_shared<Event>(kid));
			thread_count=0;
		}
		
		bool operator==(const Kernel &other){
			return id==other.id;
		}
		
		/*! \brief Manual buffer tlb configuration for hardware accelerators */
		ExitCode mmap_buffer(uint32_t bid, uint64_t vaddr){
			/*! \TODO Check that the requested mapping is valid */
			tlb[bid]=vaddr;
			return ExitCode::SUCCESS;
		}

		/*! \brief Manual event tlb configuration for hardware accelerators */
		ExitCode mmap_event(uint32_t eid, uint64_t vaddr){
			/*! \TODO Check that the requested mapping is valid */
			tlb_events[eid]=vaddr;
			return ExitCode::SUCCESS;
		}
		
		bool reads(uint32_t buffer_id){
			if (std::find(in.begin(), in.end(), buffer_id) != in.end())
				return true;
			else return false;
		}

		bool writes(uint32_t buffer_id){
			if (std::find(out.begin(), out.end(), buffer_id) != out.end())
				return true;
			else return false;
		}
};

#ifdef BOSP

class BBQKernel : public Kernel {
	public :
		BBQKernel(int kid, KernelFunction &k, std::vector<uint32_t> in_buffers, std::vector<uint32_t> out_buffers) { 
			in=in_buffers; 
			out=out_buffers; 
			id=kid; 
			kernel=&k; 
			event=std::make_shared<KernelCompletionEvent>(kid);
			for(int i=0; i<3; i++)
				task_events.push_back(std::make_shared<Event>(kid));
			thread_count=0;
		}
};

#endif /* BOSP */

}
#endif /* KERNEL_H */
