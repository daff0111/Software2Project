/*! \file
 *  \brief Context of the host-side runtime
 */
#ifndef CONTEXT_H
#define CONTEXT_H
#include "mm.h"
#include "kernel_arguments.h"

#ifdef UPV_HN_ENABLED
#include "hn.h"
#else
#include "process.h"
#endif /* UPV_HN_ENABLED */

namespace mango {

/*! \class Context 
 * \brief A class to hold the current state of the host-side runtime.
 */
class Context {
	public:
		MM mm;
	
		std::map<uint32_t, std::shared_ptr<Kernel>> kernels;
		std::map<uint32_t, std::shared_ptr<Buffer>> buffers;
		std::map<uint32_t, std::shared_ptr<Event>> events;

		/*! \brief Initialize runtime library */
		Context() : mm() { 
			Event::gn_init();	
			std::cerr << "Semaphore " << (uint64_t)Event::semaphore << std::endl;
		};

		/*! \brief Shutdown runtime library */ 
		~Context() {
			std::cerr << "mango shutdown" << std::endl;
			print(__FUNCTION__,__LINE__);
			kernels.clear();
			buffers.clear();
			events.clear();
			sem_unlink("sem_mango");
			print(__FUNCTION__,__LINE__); 
		};

		/*! \brief Support function for printing out the state of the context 
		 * \param s set to __FUNCTION__
		 * \param l set to __LINE__
		 */
		void print(const char *s, int l){
			std::cerr << s << " " << l << std::endl;
			std::cerr << "\tkernels ";
			for (auto it = kernels.cbegin(); it!= kernels.cend(); ++it)
				std::cerr << it->first << " (" << it->second.use_count() << ") ";
			std::cerr << std::endl;
			std::cerr << "\tbuffers ";
			for (auto it = buffers.cbegin(); it!= buffers.cend(); ++it)
				std::cerr << it->first << " ("  << it->second.use_count() << ") ";
			std::cerr << std::endl;
			std::cerr << "\tevents ";
			for (auto it = events.cbegin(); it!= events.cend(); ++it)
				std::cerr << it->first << " ("  << it->second.use_count() << ") ";
			std::cerr << std::endl;
		}

		/*! \brief Resource Allocation for a task graph of the application
		 * \param tg The task graph to allocate resources for
		 * \returns An exit code signalling the correct allocation (or not)
		 * \note Current implementation is a dummy.
		 */
		ExitCode resource_allocation(TaskGraph &tg){ 
			/* Dummy assignment of GN units to all kernels */
			#ifdef UPV_HN_ENABLED
			int i=0, j=0;
			hn_st_req request;
			request.num_comp_rsc=tg.kernels.count();
			request.num_mem_buffers=tg.buffers.count();
			for (i=0; i<request.num_comp_rsc; i++){
				/* Doesn't make sense, we'd have to iterate... */
				request.comp_rsc_types[i]=HN_PEAK_TYPE_1; 
			}
			for (auto& b : tg.buffers){
				request.comp_buffer_size[i]=b->size;
				i++;
			}
			i=0; 
			for(auto& k : tg.kernels){
				j=0;
				for (auto& b : tg.buffers){
					if (k.reads(b->id)) request.bw_read_req[i][j]=1;
					else request.bw_read_req[i][j]=0;
					j++;
				}
				i++;
			}
			i=0; 
			for(auto& k : tg.kernels){
				j=0;
				for (auto& b : tg.buffers){
					if (k.writes(b->id)) request.bw_write_req[i][j]=1;
					else request.bw_write_req[i][j]=0;
					j++;
				}
				i++;
			}
			uint32_t partition;
			hn_find_partition(&request, &partition);
			hn_allocate_partition(partition);
			i=0;
			for(auto& k : tg.kernels){
				std::cout << "Assigning unit "<< request.comp_rsc_tiles[i] <<" to kernel " << k->id << std::endl;
				kernels[k->id]->unit=std::make_shared<Unit>(1);		
			}			
			#else
			/* \todo Replace dummy resource allocation with BBQ call */
			for(auto& k : tg.kernels){
				std::cout << "Assigning unit GN to kernel " << k->id << std::endl;
				kernels[k->id]->unit=std::make_shared<Unit>(1);
			}
			#endif /* UPV_HN_ENABLED */
			/* Dummy memory management */
			mm.allocate_buffers(tg, buffers);
			mm.allocate_events(tg, events);
			/* End replace with BBQ call */
			
			/* TLB management */
			mm.set_vaddr_buffers(tg, buffers, kernels);
			mm.set_vaddr_events(tg, events, kernels);

			#ifdef UPV_HN_ENABLED
			i=0;
			for (auto& b : tg.buffers) {
				buffers[b->id]->phy_addr=request.mem_buffers_addr[i];
				buffers[b->id]->gn_addr=request.mem_buffers_addr[i];
				i++;
			}
			#endif /* UPV_HN_ENABLED */

			/* Initialize all buffer locks to WRITE */
			for (auto &b : tg.buffers) 
				b->event->write(2);
			return ExitCode::SUCCESS;
		};

		/*! \brief Resource Allocation for a task graph of the application
		 * \param tg The task graph to deallocate resources for
		 * \note Current implementation is a dummy
		 */
		void resource_deallocation(TaskGraph &tg){
			mm.deallocate_buffers(tg, buffers);
		};

		/*! \brief Start a given kernel
		 * \param kernel Pointer to the Kernel to start
		 * \param args A KernelArguments object which provides the marshalling of
		 * arguments
		 * \return A pointer to an Event, which is notified upon completion
		 */
		std::shared_ptr<Event> start_kernel(std::shared_ptr<Kernel> kernel, KernelArguments &args){
			if (kernel->unit->arch!=UnitType::GN) throw std::exception();
			return gn_start_kernel(kernel, args, kernel->event);
		};


		/*! \brief Start a given kernel
		 * \param kernel Pointer to the Kernel to start
		 * \param args A KernelArguments object which provides the marshalling of
		 * arguments
		 * \param event A pointer to an Event, which is notified upon completion
		 * \return The same pointer to Event received as parameter
		 */
		std::shared_ptr<Event> start_kernel(
			std::shared_ptr<Kernel> kernel, 
			KernelArguments &args, 
			std::shared_ptr<Event> event) {
			if (kernel->unit->arch!=UnitType::GN) throw std::exception();
			return gn_start_kernel(kernel, args, event);
		};
		
		
		/*! \brief Simple implementation for GN. */
		std::shared_ptr<Event> gn_start_kernel(
			std::shared_ptr<Kernel> kernel, 
			KernelArguments &args,
			std::shared_ptr<Event> event) {
			pid_t pid = gn_execvp(
				(char *)kernel->kernel->version[UnitType::GN], 
				args.gn_args()
			);
			event->gn_kernel_out=std::make_unique<std::future<uint32_t>>(
				std::async(gn_wait, pid));
			return event;
		}
		
		Context & operator+=(std::shared_ptr<Kernel> k){
			kernels.emplace(k->id,k);
			return *this;
		}

		Context & operator+=(std::shared_ptr<Buffer> b){
			buffers.emplace(b->id,b);
			return *this;
		}

		Context & operator+=(std::shared_ptr<Event> e){
			events.emplace(e->id,e);
			return *this;
		}

		std::shared_ptr<Kernel> register_kernel(uint32_t kid, KernelFunction &k, std::initializer_list<uint32_t> in_buffers={}, std::initializer_list<uint32_t> out_buffers={}){
			std::shared_ptr<Kernel> the_kernel = std::make_shared<mango::Kernel>(kid, k, std::vector<uint32_t>(in_buffers), std::vector<uint32_t>(out_buffers));
			kernels.emplace(kid, the_kernel);
			events.emplace(the_kernel->event->id,the_kernel->event);
			for(auto e : the_kernel->task_events)
				events.emplace(e->id,e);
			print(__FUNCTION__,__LINE__);			
			return the_kernel;
		}

		std::shared_ptr<Kernel> register_kernel(uint32_t kid, KernelFunction &k, std::vector<uint32_t> in_buffers, std::vector<uint32_t> out_buffers){
			std::shared_ptr<Kernel> the_kernel = std::make_shared<mango::Kernel>(kid, k, in_buffers, out_buffers);
			kernels.emplace(kid, the_kernel);
			events.emplace(the_kernel->event->id,the_kernel->event);
			for(auto e : the_kernel->task_events)
				events.emplace(e->id,e);
			print(__FUNCTION__,__LINE__);			
			return the_kernel;
		}
		
		template<class T> 
		std::shared_ptr<T> register_buffer(uint32_t bid, size_t size, std::initializer_list<uint32_t> in={}, std::initializer_list<uint32_t> out={}){
			std::shared_ptr<T> the_buffer = std::make_shared<T>(bid, size, std::vector<uint32_t>(in), std::vector<uint32_t>(out));
			std::cerr << "Buffer " << bid << " event " << the_buffer->event->id << std::endl;
			buffers.emplace(bid, the_buffer);
			events.emplace(the_buffer->event->id,the_buffer->event);
			print(__FUNCTION__,__LINE__);
			return the_buffer;
		}

		template<class T> 
		std::shared_ptr<T> register_buffer(uint32_t bid, size_t size, std::vector<uint32_t> in, std::vector<uint32_t> out){
			std::shared_ptr<T> the_buffer = std::make_shared<T>(bid, size, in, out);
			std::cerr << "Buffer " << bid << " event " << the_buffer->event->id << std::endl;
			buffers.emplace(bid, the_buffer);
			events.emplace(the_buffer->event->id,the_buffer->event);
			print(__FUNCTION__,__LINE__);
			return the_buffer;
		}

		void deregister_buffer(uint32_t bid){
			events.erase(buffers[bid]->event->id);
			buffers.erase(bid);
			print(__FUNCTION__,__LINE__);			
		}

		std::shared_ptr<Event> register_event(std::initializer_list<uint32_t> in_buffers, std::initializer_list<uint32_t> out_buffers){
			std::shared_ptr<Event> the_event = std::make_shared<mango::Event>( std::vector<uint32_t>(in_buffers), std::vector<uint32_t>(out_buffers));
			events.emplace(the_event->id, the_event);
			return the_event;
		}

		std::shared_ptr<Event> register_event(std::shared_ptr<Event> event){
			events.emplace(event->id, event);
			return event;
		}
};


#ifdef BOSP
}
#include "pmsl/app_controller.h"
#include "tg/task_graph.h"
#include <string>
namespace mango {

class BBQContext : public Context {
	bbque::ApplicationController app_ctrl;
	std::shared_ptr<bbque::TaskGraph> btg;
	
	public:	
			/*! \brief Initialize runtime library */
		BBQContext(
			std::string const & _name = "app", 
			std::string const & _recipe = "generic") : 
			Context(), app_ctrl(_name,_recipe) { 
				app_ctrl.Init(); 
		}

		~BBQContext() {
			btg->Print();
			app_ctrl.WaitForTermination();
			kernels.clear();
			buffers.clear();
			events.clear();
		}

		std::shared_ptr<bbque::TaskGraph> to_bbque(TaskGraph &tg){
			auto kl = bbque::TaskMap_t();
			auto bl = bbque::BufferMap_t();
			auto el = bbque::EventMap_t();
			for(auto &k : tg.kernels){
				std::list<uint32_t> inl(k->in.begin(), k->in.end());
				std::list<uint32_t> outl(k->out.begin(), k->out.end());
				auto bk = boost::make_shared<bbque::Task>(k->id, inl, outl);
				kl[k->id]=bk;
			}
			for(auto &b : tg.buffers){
				auto bb = boost::make_shared<bbque::Buffer>(b->id, b->size);
				bb->SetEvent(b->event->id);
				bl[b->id]=bb;
			}
			for(auto &e : tg.events){
				auto be = boost::make_shared<bbque::Event>(e->id);
				el[e->id]=be;
			}
			btg = std::make_shared<bbque::TaskGraph>(kl,bl,el);
			for(auto &b : tg.buffers){
				//Note that here we set multiple output buffers...
				if (b->out.size()==0 || b->isReadByHost()){
					std::cerr << "bbque output buffer " << b->id << std::endl;
					btg->SetOutputBuffer(b->id);
					b->event->set_callback(
						&bbque::ApplicationController::NotifyEvent,
						app_ctrl,
						b->id);
				}
			}
			btg->Print();
			return btg;
		};

		void from_bbque(TaskGraph &tg){
			for(auto k : btg->Tasks()){
				uint32_t pid = k.second->GetMappedProcessor();
				std::cerr << "Assigning unit GN to kernel " << k.first << std::endl;
				for(auto &kt : tg.kernels)
					if (k.first==kt->id) {
						std::cout << " (Unit 1) processor " << pid << std::endl;
						kt->unit=std::make_shared<Unit>(1);	
						kt->unit->id=pid;
					}
			}
		}

		ExitCode resource_allocation(TaskGraph &tg){ 
			to_bbque(tg);
			app_ctrl.GetResourceAllocation(btg);
			/* Dummy memory management - this should be done on BBQ side */
			mm.allocate_buffers(tg, buffers);
			mm.allocate_events(tg, events);
			/* End dummy */
			from_bbque(tg);
			btg->Print();
			/* TLB management */
			mm.set_vaddr_buffers(tg, buffers, kernels);
			mm.set_vaddr_events(tg, events, kernels);

			/* Initialize all buffer locks to WRITE */
			for (auto &b : tg.buffers) 
				b->event->write(2);
			return ExitCode::SUCCESS;
		}

		std::shared_ptr<KernelCompletionEvent> start_kernel(
			std::shared_ptr<Kernel> kernel, 
			KernelArguments &args) {
				app_ctrl.NotifyTaskStart(kernel->id);
				auto e = std::static_pointer_cast<mango::KernelCompletionEvent>(
					Context::start_kernel(kernel, args));
				e->set_callback(
					&bbque::ApplicationController::NotifyTaskStop,
					app_ctrl,
					kernel->id);
				btg->Print();
				return e;
		};

		std::shared_ptr<Kernel> register_kernel(uint32_t kid, KernelFunction &k, std::initializer_list<uint32_t> in_buffers={}, std::initializer_list<uint32_t> out_buffers={}){
			std::shared_ptr<mango::BBQKernel> the_kernel =
				std::make_shared<mango::BBQKernel>(
					kid, k,
					std::vector<uint32_t>(in_buffers),
					std::vector<uint32_t>(out_buffers));
			kernels.emplace(kid, the_kernel);
			events.emplace(the_kernel->event->id,the_kernel->event);
			for(auto e : the_kernel->task_events)
				events.emplace(e->id,e);
			print(__FUNCTION__,__LINE__);			
			return the_kernel;
		}

		std::shared_ptr<Kernel> register_kernel(uint32_t kid, KernelFunction &k, std::vector<uint32_t> in_buffers, std::vector<uint32_t> out_buffers){
			std::shared_ptr<mango::BBQKernel> the_kernel =
				std::make_shared<mango::BBQKernel>(
					kid, k,
					std::vector<uint32_t>(in_buffers),
					std::vector<uint32_t>(out_buffers));
			kernels.emplace(kid, the_kernel);
			events.emplace(the_kernel->event->id,the_kernel->event);
			for(auto e : the_kernel->task_events)
				events.emplace(e->id,e);
			print(__FUNCTION__,__LINE__);			
			return the_kernel;
		}
};
#endif /* BOSP */
	
} //namespace mango
#endif /* CONTEXT_H */
