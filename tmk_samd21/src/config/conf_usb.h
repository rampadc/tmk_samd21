#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "compiler.h"

/**
* USB Device Configuration
* @{
*/

//! Device definition (mandatory)
#define  USB_DEVICE_VENDOR_ID             USB_VID_ATMEL
#define  USB_DEVICE_PRODUCT_ID            0xFFFF
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         0
#define  USB_DEVICE_POWER                 100 // Consumption on VBUS line (mA)
#define  USB_DEVICE_ATTR                  (USB_CONFIG_ATTR_REMOTE_WAKEUP|USB_CONFIG_ATTR_SELF_POWERED)

//! USB Device string definitions (Optional)
#define  USB_DEVICE_MANUFACTURE_NAME      "Atmel"
#define  USB_DEVICE_PRODUCT_NAME          "USB HIDs and CDC"

/**
* Device speeds support
* @{
*/
//! To define a Low speed device
//#define  USB_DEVICE_LOW_SPEED

/**
* USB Device Callbacks definitions (Optional)
* @{
*/
#define  UDC_VBUS_EVENT(b_vbus_high)      usb_callback_vbus_action(b_vbus_high)
#define  UDC_SOF_EVENT()                  usb_callback_sof_action()
#define  UDC_SUSPEND_EVENT()              usb_callback_suspend_action()
#define  UDC_RESUME_EVENT()               usb_callback_resume_action()
#define  UDC_REMOTEWAKEUP_ENABLE()        usb_callback_remotewakeup_enable()
#define  UDC_REMOTEWAKEUP_DISABLE()       usb_callback_remotewakeup_disable()
#define  UDC_GET_EXTRA_STRING()						usb_callback_extra_string()

extern void usb_callback_vbus_action(bool b_vbus_high);
extern void usb_callback_sof_action(void);
extern void usb_callback_suspend_action(void);
extern void usb_callback_resume_action(void);
extern void usb_callback_remotewakeup_enable(void);
extern void usb_callback_remotewakeup_disable(void);
extern bool usb_callback_extra_string(void);
//@}

/**
* USB Device low level configuration
* When only one interface is used, these configurations are defined by the class module.
* For composite device, these configuration must be defined here
* @{
*/
//! Control endpoint size
#define  USB_DEVICE_EP_CTRL_SIZE       64

//! Number of interfaces for this device
// CDC: 2, HID Kbd: 1, HID M: 1
#define  USB_DEVICE_NB_INTERFACE       4 

//! Total endpoint used by all interfaces
//! Note:
//! It is possible to define an IN and OUT endpoints with the same number on XMEGA product only
//! E.g. MSC class can be have IN endpoint 0x81 and OUT endpoint 0x01
// CDC: 3, HID Kbd: 1, HID M: 1
#define  USB_DEVICE_MAX_EP             5
//@}

//@}


/**
* USB Interface Configuration
* @{
*/

/**
* Configuration of CDC interface (if used)
* @{
*/

//! Number of communication port used (1 to 3)
#define  UDI_CDC_PORT_NB 1

//! Interface callback definition
#define UDI_CDC_ENABLE_EXT(port)					cdc_callback_enable()
#define UDI_CDC_DISABLE_EXT(port)					cdc_callback_disable()
#define	UDI_CDC_RX_NOTIFY(port)						cdc_callback_rx_notify(port)
#define UDI_CDC_TX_EMPTY_NOTIFY(port)			cdc_callback_tx_empty_notify(port)
#define UDI_CDC_SET_CODING_EXT(port,cfg)
#define UDI_CDC_SET_DTR_EXT(port,set)			cdc_callback_set_dtr(port,set)
#define	UDI_CDC_SET_RTS_EXT(port,set)			cdc_callback_set_rts(port,set)

extern bool cdc_callback_enable(void);
extern void cdc_callback_disable(void);
extern void cdc_callback_rx_notify(uint8_t port);
extern void cdc_callback_tx_empty_notify(uint8_t port);
extern void cdc_callback_set_dtr(uint8_t port, bool b_enable);
extern void cdc_callback_set_rts(uint8_t port, bool b_enable);

//! Define it when the transfer CDC Device to Host is a low rate (<512000 bauds)
//! to reduce CDC buffers size
#define  UDI_CDC_LOW_RATE

//! Default configuration of communication port
#define  UDI_CDC_DEFAULT_RATE             115200
#define  UDI_CDC_DEFAULT_STOPBITS         CDC_STOP_BITS_1
#define  UDI_CDC_DEFAULT_PARITY           CDC_PAR_NONE
#define  UDI_CDC_DEFAULT_DATABITS         8

/**
* USB CDC low level configuration
* In standalone these configurations are defined by the CDC module.
* For composite device, these configuration must be defined here
* @{
*/
//! Endpoints' numbers used by single or first CDC port
#define  UDI_CDC_DATA_EP_IN_0          (1 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_0         (2 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_0             (3 | USB_EP_DIR_IN)  // Notify endpoint

//! Interface numbers used by single or first CDC port
#define  UDI_CDC_COMM_IFACE_NUMBER_0   0
#define  UDI_CDC_DATA_IFACE_NUMBER_0   1
//@}
//@}

/**
* Configuration of HID Mouse interface (if used)
* @{
*/
//! Interface callback definition
#define UDI_HID_MOUSE_ENABLE_EXT() hid_mouse_callback_enable()
#define UDI_HID_MOUSE_DISABLE_EXT() hid_mouse_callback_disable()

extern bool hid_mouse_callback_enable(void);
extern void hid_mouse_callback_disable(void);

/**
* USB HID Mouse low level configuration
* In standalone these configurations are defined by the HID Mouse module.
* For composite device, these configuration must be defined here
* @{
*/
//! Endpoint numbers definition
#define  UDI_HID_MOUSE_EP_IN           (4 | USB_EP_DIR_IN)

//! Interface number
#define  UDI_HID_MOUSE_IFACE_NUMBER    2
//@}
//@}


/**
* Configuration of HID Keyboard interface (if used)
* @{
*/
//! Interface callback definition

#define UDI_HID_KBD_ENABLE_EXT() hid_kbd_callback_enable()
#define UDI_HID_KBD_DISABLE_EXT() hid_kbd_callback_disable()
#define UDI_HID_KBD_CHANGE_LED(value) hid_kbd_callback_led(value)

extern bool hid_kbd_callback_enable(void);
extern void hid_kbd_callback_disable(void);
extern void hid_kbd_callback_led(uint8_t value);

/**
* USB HID Keyboard low level configuration
* In standalone these configurations are defined by the HID Keyboard module.
* For composite device, these configuration must be defined here
* @{
*/
//! Endpoint numbers definition
#define  UDI_HID_KBD_EP_IN           (5 | USB_EP_DIR_IN)

//! Interface number
#define  UDI_HID_KBD_IFACE_NUMBER    3
//@}
//@}

//... Eventually add other Interface Configuration

//@}


/**
 * Description of Composite Device
 * @{
 */
//! USB Interfaces descriptor structure
#define	UDI_COMPOSITE_DESC_T \
	usb_iad_desc_t       udi_cdc_iad; \
	udi_cdc_comm_desc_t  udi_cdc_comm; \
	udi_cdc_data_desc_t  udi_cdc_data; \
	udi_hid_mouse_desc_t udi_hid_mouse; \
	udi_hid_kbd_desc_t   udi_hid_kbd

//! USB Interfaces descriptor value for Full Speed
#define UDI_COMPOSITE_DESC_FS \
	.udi_cdc_iad   = UDI_CDC_IAD_DESC_0, \
	.udi_cdc_comm  = UDI_CDC_COMM_DESC_0, \
	.udi_cdc_data  = UDI_CDC_DATA_DESC_0_FS, \
	.udi_hid_mouse = UDI_HID_MOUSE_DESC, \
	.udi_hid_kbd   = UDI_HID_KBD_DESC

//! USB Interfaces descriptor value for High Speed
#define UDI_COMPOSITE_DESC_HS \
	.udi_cdc_iad   = UDI_CDC_IAD_DESC_0, \
	.udi_cdc_comm  = UDI_CDC_COMM_DESC_0, \
	.udi_cdc_data  = UDI_CDC_DATA_DESC_0_HS, \
	.udi_hid_mouse = UDI_HID_MOUSE_DESC, \
	.udi_hid_kbd   = UDI_HID_KBD_DESC

//! USB Interface APIs
#define	UDI_COMPOSITE_API \
	&udi_api_cdc_comm, \
	&udi_api_cdc_data, \
	&udi_api_hid_mouse, \
	&udi_api_hid_kbd
//@}

/**
* USB Device Driver Configuration
* @{
*/
//@}

//! The includes of classes and other headers must be done at the end of this file to avoid compile error

#include "udi_hid_kbd.h"
#include "udi_hid_mouse.h"
#include "udi_cdc.h"

#define UDD_CLOCK_GEN			GCLK_GENERATOR_3

#endif // _CONF_USB_H_
