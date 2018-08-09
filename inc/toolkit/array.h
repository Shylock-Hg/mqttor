#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "../../port/config.h"

///! \defgroup toolkit_array
/// @{

#define COUNT_OF_ARRAY(array) (sizeof(array)/sizeof(array[0]))

#if (configENDIAN == 0)  //!< ENDIAN_BIG
/*! \brief convert bytes[2] to uint16  
 *  \note bigendian
 * */
#define BYTES_2_UINT16(bytes) (((bytes)[0]<<8) | ((bytes)[1]))

/*! \brief convert uint16 to bytes[2]
 *  \note bigendian
 * */
#define UINT16_2_BYTES(num,bytes) do{\
		bytes[0] = num>>8;\
		bytes[1] = num&0x00FF;\
	}while(0)

#elif (configENDIAN == 1)  //!< ENDIAN_LITTLE

#else
	#error "undefined endian of target!"
#endif

/*! \brief caculate bit mask by offset[0:]
 * */
#define BIT(offset) (1<<(offset))

/// @}

#endif

