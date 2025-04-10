
#ifndef __KD32F3XX_H
#define __KD32F3XX_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
 
/**
  * @brief CMSIS Device version number V2.3.3
  */
#define __DEVICE_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __DEVICE_VERSION_SUB1   (0x03) /*!< [23:16] sub1 version */
#define __DEVICE_VERSION_SUB2   (0x03) /*!< [15:8]  sub2 version */
#define __DEVICE_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __DEVICE_VERSION        ((__DEVICE_VERSION_MAIN << 24)\
                                        |(__DEVICE_VERSION_SUB1 << 16)\
                                        |(__DEVICE_VERSION_SUB2 << 8 )\
                                        |(__DEVICE_VERSION_RC))
                                             

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#include "kd32f328xb.h"  
#include "kd32f328_conf.h"  

/** @addtogroup Exported_macros
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/*****************************END OF FILE****/
