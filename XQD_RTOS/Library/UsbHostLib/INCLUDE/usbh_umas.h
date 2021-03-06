/**************************************************************************//**
 * @file     usbh_umas.h
 * @version  V1.00
 * $Revision 2 $
 * $Date: 14/06/19 8:04p $
 * @brief   USB Host Mass Storage driver header file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef _USBH_UMAS_H_
#define _USBH_UMAS_H_

#ifdef __cplusplus
extern "C"
{
#endif


/// @cond HIDDEN_SYMBOLS

typedef struct                         /* USB Mass Storage disk                 */
{
    USB_DEV_T     *pusb_dev;           /* This USB device.                      */
    uint16_t      vendor_id;           /* Vendor ID in device descriptor        */
    uint16_t      product_id;          /* Vendor ID in device descriptor        */
    uint32_t      sector_size;
    uint32_t      sector_number;
} mass_disk_t;

/*----------------------------
 * APIs for FATFS
 *----------------------------*/
extern int usbh_umas_disk_status(void);
extern DRESULT usbh_umas_ioctl(int cmd, void *buff);
extern DRESULT usbh_umas_read(uint8_t *buff, uint32_t sector_no, int number_of_sector);
extern DRESULT usbh_umas_write(uint8_t *buff, uint32_t sector_no, int number_of_sector);
/// @endcond HIDDEN_SYMBOLS


/** @addtogroup M451_Device_Driver M451 Device Driver
  @{
*/

/** @addtogroup M451_USBH_MASS_Driver USB Host Mass Storage Driver
  @{
*/


/** @addtogroup M451_USBH_MASS_EXPORTED_FUNCTIONS USB Host Mass Storage Driver Exported Functions
  @{
*/

extern int32_t  USBH_MassInit(void);
extern int32_t  USBH_MassGetDiskList(mass_disk_t * dlist[], int max);
extern int32_t  USBH_MassRawRead(mass_disk_t *disk, uint32_t sectorN, int32_t scnt, uint8_t *buff);
extern int32_t  USBH_MassRawWrite(mass_disk_t *disk, uint32_t sectorN, int32_t scnt, uint8_t *buff);

/*@}*/ /* end of group M451_USBH_MASS_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M451_USBH_MASS_Driver */

/*@}*/ /* end of group M451_Device_Driver */


#ifdef __cplusplus
}
#endif

#endif

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/


