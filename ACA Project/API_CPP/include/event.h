/*! \file
 *  \brief Synchronization events
 */
#ifndef EVENT_H
#define EVENT_H
#include "mango_types.h"
#include <vector>
#include <semaphore.h> // sem_t
#include <atomic>
#include <memory>
#include <iostream>
#include <future>   // gn_kernel_out
#include <thread>
#include <cstring>  // strerror
#include <unistd.h> // usleep
#include <fcntl.h>  // O_CREAT
#include <errno.h>  // errno

#ifdef BOSP
#include "pmsl/app_controller.h"
#include <functional>
#endif 

namespace mango {

/*! \brief HN Event descriptor 
 *
 * These events are also used at the GN level.
 */
class Event {
	private :
#ifdef BOSP
		std::function<void()> 
			bbq_notify_callback;
#else
		std::function<mango::ExitCode()> 
			bbq_notify_callback;
#endif

	public:	
		static sem_t *semaphore;
		uint32_t id; /*!< event id */
		uint32_t phy_addr;  /*!< A memory offset from the start of the events area */
		uint32_t *gn_addr; /* address for use in gn memory mapped version */
		std::vector<uint32_t> in;	/*!< Writer kernels */
		std::vector<uint32_t> out; /*!< Reader kernels */
		std::unique_ptr<std::future<uint32_t>> gn_kernel_out;
		std::unique_ptr<std::thread> fifo_task;

		static void gn_init(){ 
			semaphore=sem_open("sem_mango",O_CREAT,0777,1); 
			if (semaphore==SEM_FAILED){ 
				std::cerr << strerror(errno) << std::endl;
			}
		}

		void wait_state(uint32_t state){
			uint32_t value;
			do {
				value=lock();
				write(value);
				if (value!=state){ 
					usleep(1);
				}
			}	while(value!=state);
		}

	/*! \brief High level wait primitive
	 */
		uint32_t wait(){
			if (gn_kernel_out) {
				wait_state(1);
				uint32_t es = gn_kernel_out->get();
				std::cerr << "End kernel with status " << es << std::endl;
				return es;
			}
			else if (fifo_task) {
				wait_state(END_FIFO_OPERATION);
				fifo_task->join();
				std::cerr << "End FIFO Ops" << std::endl;
			}
			else wait_state(1);
			if (bbq_notify_callback!=nullptr) {
				std::cerr << "Notifying buffer read callback" << std::endl;
				bbq_notify_callback();
			}
			return 0;
		};

	/*! \brief Set an event value
	 * \param value An integer value
	 * write value to the TILEREG register associated with event
	 */		
		void write(uint32_t value){
			sem_wait(semaphore);
			*gn_addr=value;
			sem_post(semaphore);
			//if (value==READ && bbq_notify_callback!=nullptr) {
			//	std::cerr << "Notifying buffer write callback" << std::endl;
			//	bbq_notify_callback();
			//}
		}

	/*! \brief Read and reset an event
	 * read value from the TILEREG register associated with event and replace it
	 * with 0
	 */		
		uint32_t read(){
			int v;
			sem_getvalue(Event::semaphore, &v);
			sem_wait(semaphore);
			uint32_t value=*gn_addr;
			*gn_addr=0;
			sem_getvalue(Event::semaphore, &v);
			sem_post(semaphore);
			return value;
		};
		
	/*! \brief lock and read event 
	 * read value from the TILEREG register associated with event and replace it
	 * with 0, when it becomes != 0 (i.e., when unlocked)
	 */
 		uint32_t lock(){
			uint32_t value;
			do {
				value = read();
			} while(value==0);
			return value;
		};
	
		bool operator==(const Event &other){
			return id==other.id;
		}

		Event(){
			id=id_gen++;
			phy_addr=id * sizeof(uint32_t);
			bbq_notify_callback=nullptr;
		}

		Event(uint32_t kernel) {
			id=id_gen++; 
			in=std::vector<uint32_t>({kernel}) ;
			out=std::vector<uint32_t>({kernel}) ;
			phy_addr=id * sizeof(uint32_t);
			bbq_notify_callback=nullptr;
		}

		Event(std::vector<uint32_t> in_k, std::vector<uint32_t> out_k){
			id=id_gen++; 
			in=in_k;
			out=out_k;
			phy_addr=id * sizeof(uint32_t);
			bbq_notify_callback=nullptr;
		}

		template<typename A, typename B>
		void set_callback(
			A	_bbq_notify_callback, 
			B obj, 
			uint32_t _id){
			bbq_notify_callback=std::bind(_bbq_notify_callback, obj, id);
		}
	
  	static std::atomic<int> id_gen;
};

#ifdef BOSP
class KernelCompletionEvent : public Event {
	private :
		std::function<bbque::ApplicationController::ExitCode()> 
			bbq_notify_callback;
	public :
		KernelCompletionEvent(uint32_t kernel) : Event(kernel){};
		
		template<typename A, typename B>
		void set_callback(
			A	_bbq_notify_callback, 
			B obj,
			uint32_t kernel_id){
			bbq_notify_callback=std::bind(_bbq_notify_callback, obj, kernel_id);
		}

		uint32_t wait(){
			uint32_t val = Event::wait();
			std::cerr << "Notifying end of kernel callback" << std::endl;
			bbq_notify_callback();
			return val;
		};
};

#endif /* BOSP */

}
#endif /* EVENT_H */
