/*
 * mausb_common.h
 * command structure borrowed from udev
 * (git://git.kernel.org/pub/scm/linux/hotplug/udev.git)
 *
 * Copyright (C) 2011 matt mooney <mfm@muteddisk.com>
 *               2005-2007 Takahiro Hirofuchi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MAUSB_COMMON_H
#define __MAUSB_COMMON_H

//#include <libsysfs.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#ifndef USBIDS_FILE
#define USBIDS_FILE "/usr/share/hwdata/usb.ids"
#endif

/* kernel module names */
#define MAUSB_CORE_MOD_NAME    "mausb-core"
#define MAUSB_HOST_DRV_NAME    "mausb-host"

#define SYSFS_DRIVERS_NAME    "drivers"
#define SYSFS_BUS_NAME        "bus"
#define SYSFS_PATH_MAX        256
#define SYSFS_BUS_ID_SIZE    32

#define ATTR_BUFFER        32
#define CONF_VALUE_BUFFER    32
#define NUM_INF_BUFFER        32
#define BUFFER_SIZE        100
#define SOCKET_BUFFER        30

extern int mausb_use_syslog;
extern int mausb_use_stderr;
extern int mausb_use_debug ;

#define PROGNAME "mausb"

#define pr_fmt(fmt)    "%s: %s: " fmt "\n", PROGNAME
#define dbg_fmt(fmt)    pr_fmt("%s:%d:[%s] " fmt), "debug",    \
                __FILE__, __LINE__, __FUNCTION__

#define err(fmt, args...)                        \
    do {                                \
        if (mausb_use_syslog) {                    \
            syslog(LOG_ERR, pr_fmt(fmt), "error", ##args);    \
        }                            \
        if (mausb_use_stderr) {                    \
            fprintf(stderr, pr_fmt(fmt), "error", ##args);    \
        }                            \
    } while (0)

#define info(fmt, args...)                        \
    do {                                \
        if (mausb_use_syslog) {                    \
            syslog(LOG_INFO, pr_fmt(fmt), "info", ##args);    \
        }                            \
        if (mausb_use_stderr) {                    \
            fprintf(stderr, pr_fmt(fmt), "info", ##args);    \
        }                            \
    } while (0)

#define dbg(fmt, args...)                        \
    do {                                \
    if (mausb_use_debug) {                        \
        if (mausb_use_syslog) {                    \
            syslog(LOG_DEBUG, dbg_fmt(fmt), ##args);    \
        }                            \
        if (mausb_use_stderr) {                    \
            fprintf(stderr, dbg_fmt(fmt), ##args);        \
        }                            \
    }                                \
    } while (0)

#define BUG()                        \
    do {                        \
        err("sorry, it's a bug!");        \
        abort();                \
    } while (0)

enum usb_device_speed {
    USB_SPEED_UNKNOWN = 0,                  /* enumerating */
    USB_SPEED_LOW, USB_SPEED_FULL,          /* usb 1.1 */
    USB_SPEED_HIGH,                         /* usb 2.0 */
    USB_SPEED_VARIABLE                      /* wireless (usb 2.5) */
};

/* FIXME: how to sync with drivers/mausb_common.h ? */
enum mausb_device_status{
    /* sdev is available. */
    SDEV_ST_AVAILABLE = 0x01,
    /* sdev is now used. */
    SDEV_ST_USED,
    /* sdev is unusable because of a fatal error. */
    SDEV_ST_ERROR,

    /* vdev does not connect a remote device. */
    VDEV_ST_NULL,
    /* vdev is used, but the USB address is not assigned yet */
    VDEV_ST_NOTASSIGNED,
    VDEV_ST_USED,
    VDEV_ST_ERROR
};

struct mausb_usb_interface {
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t padding;    /* alignment */
} __attribute__((packed));

struct mausb_usb_device {
    char path[SYSFS_PATH_MAX];
    char busid[SYSFS_BUS_ID_SIZE];

    uint32_t busnum;
    uint32_t devnum;
    uint32_t speed;

    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;

    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bConfigurationValue;
    uint8_t bNumConfigurations;
    uint8_t bNumInterfaces;
} __attribute__((packed));

#define to_string(s)    #s

void dump_usb_interface(struct mausb_usb_interface *);
void dump_usb_device(struct mausb_usb_device *);
int read_usb_device(char *sdevpath, struct mausb_usb_device *udev);
int read_attr_value(char *sdevpath, const char *name, const char *format);
int read_usb_interface(struct mausb_usb_device *udev, int i,
               struct mausb_usb_interface *uinf);

const char *mausb_speed_string(int num);
const char *mausb_status_string(int32_t status);

int mausb_names_init(char *);
void mausb_names_free(void);
void mausb_names_get_product(char *buff, size_t size, uint16_t vendor,
                 uint16_t product);
void mausb_names_get_class(char *buff, size_t size, uint8_t class,
               uint8_t subclass, uint8_t protocol);
void mausb_read_attr(const char *attr_name,const char *attr_value);
void mausb_write_attr(const char *attr_name,const char *attr_value);

#endif /* __MAUSB_COMMON_H */
