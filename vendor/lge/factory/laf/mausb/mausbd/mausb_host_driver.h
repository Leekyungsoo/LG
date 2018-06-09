/*
 * mausb_host_driver.h
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

#ifndef __MAUSB_HOST_DRIVER_H
#define __MAUSB_HOST_DRIVER_H

#include <stdint.h>
#include "mausb_common.h"

struct mausb_host_driver {
    int ndevs;
    struct sysfs_driver *sysfs_driver;
    /* list of exported device */
    struct dlist *edev_list;
};

struct mausb_exported_device {
    //struct sysfs_device *sudev;
    char *sdevpath;
    int32_t status;
    struct mausb_usb_device udev;
    struct mausb_usb_interface uinf[];
};

extern struct mausb_host_driver *host_driver;

int mausb_host_driver_open(void);
void mausb_host_driver_close(void);

int mausb_host_refresh_device_list(void);
int mausb_host_export_device(struct mausb_exported_device *edev, int sockfd);
void mausb_exported_device_delete(void *dev);
struct mausb_exported_device *mausb_host_get_device(int num);
struct mausb_exported_device *mausb_exported_device_store(char *sdevpath);

#endif /* __MAUSB_HOST_DRIVER_H */
