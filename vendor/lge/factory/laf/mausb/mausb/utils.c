/*
 * utils.c
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

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "mausb_common.h"
#include "utils.h"

int modify_match_busid(char *busid, int add)
{
    //char bus_type[] = "usb";
    char attr_name[] = "match_busid";
    char buff[SYSFS_BUS_ID_SIZE + 4];
    //char sysfs_mntpath[SYSFS_PATH_MAX];
    //char match_busid_attr_path[SYSFS_PATH_MAX];
    //struct sysfs_attribute *match_busid_attr;
    int ret = 0;

    if (add)
        snprintf(buff, SYSFS_BUS_ID_SIZE + 4, "add %s", busid);
    else
        snprintf(buff, SYSFS_BUS_ID_SIZE + 4, "del %s", busid);

    dbg("write %s", buff);
    mausb_write_driver_attr(attr_name,buff);
    return ret;
}
