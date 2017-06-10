/*! \file
 *  \brief Buffer (memory objects)
 */
#ifndef BUFFER_H
#define BUFFER_H
#include "event.h"
#include <algorithm>
#include <sys/mman.h>

namespace mango {

/*! \brief HN shared memory buffer descriptor */
class Buffer {
	public: 
		uint32_t id;  /*!< buffer id */
		uint64_t phy_addr;  /*!< Physical address of buffer in HN memory */
		uint8_t *gn_addr;
		size_t size;  /*!< Size of the memory buffer */
		std::shared_ptr<Event> event; /*!< Synchronization event */
		std::vector<uint32_t> in;	/*!< Writer kernels */
		std::vector<uint32_t> out; /*!< Reader kernels */
		
		Buffer(){};
		
		Buffer(uint32_t bid, size_t _size, std::vector<uint32_t> _in={}, std::vector<uint32_t> _out={}){
			size=_size;
			id=bid;
			in=_in;
			out=_out;
			event=std::make_shared<Event>(in, out);
		}

	/*! \brief Memory transfer from GN to HN in DIRECT mode
	 * \param GN_buffer A pointer to memory in the GN address space
	 * This function performs a copy between a memory region in the GN address space
   * and one in the HN address space. The copy works on the entire buffer size
	 * specified in the HN buffer descriptor.
	 * \note Current specification assumes synchronous transfer
	 */
		std::shared_ptr<Event> write(void *GN_buffer){
			#ifdef UPV_HN_ENABLED
			uint32 hn_write_memory(0, phy_addr, size, GN_buffer);
			#else
			msync(gn_addr, size, MS_SYNC);
			std::memcpy((char *)gn_addr, (char *)GN_buffer, size);
			msync(gn_addr, size, MS_SYNC);
			#endif /* UPV_HN_ENABLED */
			return event;
		};

	/*! \brief Memory transfer from HN to GN in DIRECT mode
	 * \param GN_buffer A pointer to memory in the GN address space
	 * This function performs a copy between a memory region in the HN address space
	 * and one in the GN address space. The copy works on the entire buffer size
	 * specified in the HN buffer descriptor.	
	 * \note Current specification assumes synchronous transfer
	 */
		std::shared_ptr<Event> read(void *GN_buffer){
			#ifdef UPV_HN_ENABLED
			uint32 hn_read_memory(0, phy_addr, size, GN_buffer);
			#else
			std::cerr << "Read " << size << " bytes" << std::endl;
			msync(gn_addr, size, MS_SYNC);
			std::memcpy((char *)GN_buffer, (char *)gn_addr, size);
			msync(gn_addr, size, MS_SYNC);
			#endif /* UPV_HN_ENABLED */
			return event;
		};

		bool isReadByHost(){
			return isReadBy(0);
		}
		
		bool isReadBy(uint32_t kid){
			if (std::find(out.begin(), out.end(), kid) != out.end()) return true;
			return false;
		}
		
		bool operator==(const Buffer &other){
			return id==other.id;
		}
		
		mango::ExitCode resize(size_t _size){
			size=_size;
			return mango::ExitCode::SUCCESS; 
			/*! \TODO fix this, as it should not be possible to resize while buffer 
					is in use */
		}
		
};

/*! \brief FIFO-style memory buffer 
 * These buffers implement a burst-mode data transfer. The transfer is asynchronous,
 * and the entire GN-side area is transferred transparently. Of course, users should
 * avoid modifying the GN-side buffer while the transfer is ongoing.
 * \todo Implement read...
 */
class FIFOBuffer: public Buffer {
	public :
		FIFOBuffer(uint32_t bid, size_t size, std::vector<uint32_t> in,
			std::vector<uint32_t> out) : Buffer(bid, size, in, out) {};
		
		uint32_t synch_write(void *GN_buffer, size_t global_size){
			uint32_t off;
			for(off=0; off<global_size; off+=size){
				event->wait_state(WRITE);
				std::cout << "Writing " << (char *)GN_buffer << std::endl;
				std::memcpy((char *)gn_addr, (char *)GN_buffer+off, size);
				event->write(READ);
			}
			return off;
		};

		uint32_t synch_read(void *GN_buffer, size_t global_size){
			uint32_t off;
			for(off=0; off<global_size; off+=size){
				event->wait_state(READ);
				std::cout << "Reading " << (char *)GN_buffer << std::endl;
				std::memcpy((char *)GN_buffer+off, (char *)gn_addr, size);
				event->write(WRITE);
			}
			return off;
		};
		
	/*! \brief Memory transfer from GN to HN in BURST mode
	 * \param GN_buffer A pointer to memory in the GN address space
	 * \param size The global size
	 * This function performs a copy between a memory region in the GN address space
   * and one in the HN address space. The copy works in bursts of the size
	 * specified in the HN buffer descriptor.
	 * \note Current specification assumes asynchronous transfer
	 */
		std::shared_ptr<Event> write(void *GN_buffer, size_t global_size){
			event->fifo_task = std::make_unique<std::thread>(
					&FIFOBuffer::synch_write, 
					this, GN_buffer, global_size);
			return event;
		};
		
		std::shared_ptr<Event> read(void *GN_buffer, size_t global_size){
			event->fifo_task = std::make_unique<std::thread>(
					&FIFOBuffer::synch_read, 
					this, GN_buffer, global_size);
			return event;
		};
};


}
#endif /* BUFFER_H */
