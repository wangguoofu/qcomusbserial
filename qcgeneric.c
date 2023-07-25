#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/sysrq.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>
#include <linux/uaccess.h>
#include <linux/kfifo.h>
#include <linux/serial.h>

//static int generic_probe(struct usb_interface *interface,
//			 const struct usb_device_id *id);

static __u16 vendor  = 0x05c6;
static __u16 product = 0x9025;

module_param(vendor, ushort, 0);
MODULE_PARM_DESC(vendor, "User specified USB idVendor");

module_param(product, ushort, 0);
MODULE_PARM_DESC(product, "User specified USB idProduct");

void usb_serial_generic_release(struct usb_serial *serial)
{
	printk("%s", __func__);
}

static int qcprobe(struct usb_serial *serial, const struct usb_device_id *id)
{
  struct usb_host_interface *intf = serial->interface->cur_altsetting;
  
  //if(intf->endpoint[0].desc.bEndpointAddress!=0x86)
  if(intf->desc.bInterfaceProtocol!=8)
    return -ENODEV;
  else
    return 0;
}

static struct usb_device_id generic_device_ids[2]=
  {
    [0]={
      .idVendor = 0x05c6,
      .idProduct = 0x9025,
      .match_flags = USB_DEVICE_ID_MATCH_VENDOR | USB_DEVICE_ID_MATCH_PRODUCT,
    },
  }; /* Initially all zeroes. */

/* we want to look at all devices, as the vendor/product id can change
 * depending on the command line argument */
static const struct usb_device_id generic_serial_ids[] = {
	{.driver_info = 42},
	{}
};

//static struct usb_driver generic_driver = {
//	.name =		"qcusbserial_generic",
//	.probe =	generic_probe,
//	.disconnect =	usb_serial_disconnect,
//	.id_table =	generic_serial_ids,
//	.no_dynamic_id =	1,
//};

/* All of the device info needed for the Generic Serial Converter */
struct usb_serial_driver usb_serial_generic_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"qcgeneric",
	},
	.id_table =		generic_device_ids,
	//.usb_driver = 		&generic_driver,
	.num_ports =		1,
	.probe =                qcprobe,
	//.disconnect =		usb_serial_generic_disconnect,
	.release =		usb_serial_generic_release,
	.throttle =		usb_serial_generic_throttle,
	.unthrottle =		usb_serial_generic_unthrottle,
	.resume =		usb_serial_generic_resume,
};

static struct usb_serial_driver * const serial_drivers[] = {
	&usb_serial_generic_device, NULL
};

//static int generic_probe(struct usb_interface *interface,
//			       const struct usb_device_id *id)
//{
//	const struct usb_device_id *id_pattern;
//
//	id_pattern = usb_match_id(interface, generic_device_ids);
//	if (id_pattern != NULL)
//		return usb_serial_probe(interface, id);
//	return -ENODEV;
//}

#define DRIVER_AUTHOR "wizarpos Inc"
#define DRIVER_DESC "Qualcomm USB Serial driver"

static int __init qc_serial_init(void)
{
  int retval = 0;
  
  generic_device_ids[0].idVendor = vendor;
  generic_device_ids[0].idProduct = product;
  generic_device_ids[0].bInterfaceClass = 0xff;
  generic_device_ids[0].bInterfaceSubClass = 0x00;
  generic_device_ids[0].match_flags =
    USB_DEVICE_ID_MATCH_VENDOR | USB_DEVICE_ID_MATCH_PRODUCT|USB_DEVICE_ID_MATCH_INT_CLASS|USB_DEVICE_ID_MATCH_INT_SUBCLASS;

  retval = usb_serial_register_drivers(serial_drivers,
			"qualcomm_usbserial_generic", generic_device_ids);
  /* register our generic driver with ourselves */
  //retval = usb_serial_register(&usb_serial_generic_device);
  //if (retval)
  //  goto exit;
  //retval = usb_register(&generic_driver);
  //if (retval)
  //  usb_serial_deregister(&usb_serial_generic_device);
//exit:
  return retval;
}

static void __exit qc_serial_exit(void)
{
  /* remove our generic driver */
  //usb_deregister(&generic_driver);
  //usb_serial_deregister(&usb_serial_generic_device);
  usb_serial_deregister_drivers(serial_drivers);
}

module_init(qc_serial_init);
module_exit(qc_serial_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL v2");
