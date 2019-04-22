#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

///! \defgroup mqtt_config configuration of mqttor
/// @{

/*! \brief endian of bytes
 *  \value ENDIAN_BIG(0) or ENDIAN_LITTLE(1)
 * */
#define configENDIAN 0

#define configLOG 1  //!< bool

/// @}

#ifdef __cplusplus
}
#endif

#endif  //!< _CONFIG_H_
