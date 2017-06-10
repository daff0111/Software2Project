/*! \file
 *  \brief C++ API enum types
 */
#ifndef MANGO_TYPES_H
#define MANGO_TYPES_H
#define MANGO_NR_MEM_BANKS 4

#include <cstdint>
//#include <cstddef>

namespace mango {

/*! \file mango_types.h
 * \brief Include data types used in the GN-level Mango platform run-time
 */

/*!
 * \enum mango_event_status_t
 * \brief States for events
 */
typedef enum { LOCK, READ, WRITE, END_FIFO_OPERATION } mango_event_status_t;

/*!
 * \enum ExitCode
 * \brief Exit codes supported
 */
enum class ExitCode {
	SUCCESS = 0,         /*!< Successfull return */
	ERR_INVALID_VALUE,   /*!< Invalid value specified */
	ERR_INVALID_TASK_ID, /*!< Invalid task id provided */
	ERR_INVALID_KERNEL,  /*!< Invalid kernel structure */
	ERR_FEATURE_NOT_IMPLEMENTED, /*!< The feature is not implemented yet */
	ERR_INVALID_KERNEL_FILE, /*!< Kernel file is of an unknown or invalid type */
	ERR_UNSUPPORTED_UNIT, /*!< Unit type not supported in current configuration */
	ERR_OUT_OF_MEMORY,   /*! < Out of memory error */
	ERR_SEM_FAILED,      /*!< Semaphore open failure */
	ERR_MMAP_FAILED,     /*!< mmap failure */
	ERR_FOPEN,           /*!< file open failure */
	ERR_OTHER            /*!< Generic error */
};


/*! \brief Memory types
 *
 * Employed to configure memory buffer and scalar types. 
 */
enum class BufferType { 
	NONE, /*!< No type (typically, will cause errors) */
	FIFO, /*!< FIFO behaviour (a single asynchronous read or write from the GN
	* side will result in reading or writing the entire buffer with appropriate
	* synchronization */
	BUFFER, /*!< Buffer behaviour (each read or write is independent) */
	SCALAR /*!< Scalar data type */
} ;

/*! \brief Mango computational unit (device) types
 */
enum class UnitType {
	STOP, /*!< Terminator used to close arrays of mango_unit_type_t */
	PEAK, /*!< PEAK units */
	NUP,  /*!< NU+ units */
	GN    /*!< Fall back to GN node if no other option is available */
} ;


/*! \brief Memory bank descriptor
 *
 * This provides information about a memory bank, including its size and the
 * starting physical address
 */
class MemoryBank {
	uint16_t id;        /*!< The memory bank identifier */
	uint64_t phy_addr;  /*!< The starting physical address */
	uint64_t size;      /*!< The memory bank capacity */
	uint16_t tile;      /*!< Marks the tile to which the bank is attached */
};

/*!
 * \struct Unit
 * \brief Mango computational unit descriptor
 *
 * For each unit we need to keep track of the architecture type, the id number,
 * the number of cores and the tile the unit belongs to
 */
class Unit {
	public:
	UnitType arch;  /*!< Unit architecture type */
	uint16_t id;             /*!< Unique identifier */
	uint16_t nr_cores;       /*!< Number of cores in the unit */
	MemoryBank mems[MANGO_NR_MEM_BANKS]; /*!< The memory banks array   */
	
	Unit(int i) {
		arch=UnitType::GN;
		id=i;
		nr_cores=1;
	}

	Unit(UnitType _arch, int i) {
		arch=_arch;
		id=i;
		nr_cores=4;
	}
}; 


/*! \brief Unit-level statistics and run-time information
 */
class UnitStats {
	struct {
		uint32_t period_us;  /*!< The update statistics time period */
		uint32_t idle_us;    /*!< Idle time of the unit in the period */
	} time;
	uint32_t nr_tasks; /*!< Number of tasks currently running */
};

/*! \brief Types of kernel sources/file types
 */
enum class FileType { 
	UNKNOWN_KERNEL_SOURCE_TYPE, /*!< Unknown type, fail. */
	BINARY, /*!< Binary file, ready to load and execute */
	HARDWARE, /*!< Hardware accelerator */
	STRING, /*!< String containing the kernel source code */
	SOURCE  /*!< Text file containing the kernel source code */
};



}
#endif // MANGO_TYPES_H
