/*
 * mausb_host_driver.c
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

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>

#include "mausb_common.h"
#include "mausb_host_driver.h"

#undef  PROGNAME
#define PROGNAME "libmausb"

struct mausb_host_driver *host_driver;

#define SYSFS_OPEN_RETRIES 100

/* only the first interface value is true! */
static int32_t read_attr_mausb_status(struct mausb_usb_device *udev)
{
    char attrpath[SYSFS_PATH_MAX] = {0,};
    char attrvalue[ATTR_BUFFER] = {0,};
    struct sysfs_attribute *attr;
    int value = 0;
    int rc;
    struct stat s;
    int retries = SYSFS_OPEN_RETRIES;

    /* This access is racy!
     *
     * Just after detach, our driver removes the sysfs
     * files and recreates them.
     *
     * We may try and fail to open the mausb_status of
     * an exported device in the (short) window where
     * it has been removed and not yet recreated.
     *
     * This is a bug in the interface. Nothing we can do
     * except work around it here by polling for the sysfs
     * mausb_status to reappear.
     */

    dbg("read_attr_mausb_status");
    snprintf(attrpath, SYSFS_PATH_MAX, "%s/%s:%d.%d/mausb_status",
         udev->path, udev->busid, udev->bConfigurationValue, 0);

    while (retries > 0) {
        if (stat(attrpath, &s) == 0)
            break;

        if (errno != ENOENT) {
            dbg("stat failed: %s", attrpath);
            return -1;
        }

        usleep(10000); /* 10ms */
        retries--;
    }

    if (retries == 0)
        dbg("mausb_status not ready after %d retries",
            SYSFS_OPEN_RETRIES);
    else if (retries < SYSFS_OPEN_RETRIES)
        dbg("warning: mausb_status ready after %d retries",
            SYSFS_OPEN_RETRIES - retries);

    dbg("read_attr_mausb_status attrpath %s",attrpath);
    mausb_read_attr(attrpath,attrvalue);

    dbg("read_attr_mausb_status %s",attrvalue);
    value = atoi(attrvalue);
    dbg("read_attr_mausb_status %d",value);
    return value;
}

 struct mausb_exported_device *mausb_exported_device_store(char *sdevpath)
{
    struct mausb_exported_device *edev = NULL;
    size_t size;
    int i;

    dbg("mausb_exported_device_store");
    edev = calloc(1, sizeof(*edev));
    if (!edev) {
        dbg("calloc failed");
        return NULL;
    }

    /*edev->sudev = sysfs_open_device_path(sdevpath);
    if (!edev->sudev) {
        dbg("sysfs_open_device_path failed: %s", sdevpath);
        goto err;
    }*/

    dbg("mausb_exported_device_store %s ",sdevpath);

    edev->sdevpath=sdevpath;

    dbg("mausb_exported_device_store %s ",edev->sdevpath);


    read_usb_device(edev->sdevpath, &edev->udev);

    edev->status = read_attr_mausb_status(&edev->udev);

    dbg("mausb_exported_device_store %d",edev->status);
    if (edev->status < 0)
        goto err;

    /* reallocate buffer to include usb interface data */
    size = sizeof(*edev) + edev->udev.bNumInterfaces *
        sizeof(struct mausb_usb_interface);

    edev = realloc(edev, size);
    if (!edev) {
        dbg("realloc failed");
        goto err;
    }

    dbg("mausb_exported_device_store :%d",edev->udev.bNumInterfaces);
    for (i = 0; i < edev->udev.bNumInterfaces; i++)
        read_usb_interface(&edev->udev, i, &edev->uinf[i]);

    return edev;
err:
    /*if (edev && edev->sudev)
        sysfs_close_device(edev->sudev);*/
    if (edev)
        free(edev);

    return NULL;
}

static void delete_nothing(void *unused_data)
{
    /*
     * NOTE: Do not delete anything, but the container will be deleted.
     */
    (void) unused_data;
}

void mausb_exported_device_delete(void *dev)
{
    struct mausb_exported_device *edev = dev;
    free(dev);
}

int mausb_host_export_device(struct mausb_exported_device *edev, int sockfd)
{
    char attr_name[] = "mausb_sockfd";
    char attr_path[SYSFS_PATH_MAX] = {0,};
    struct sysfs_attribute *attr;
    char sockfd_buff[SOCKET_BUFFER];
    int ret=0;
    int sendbuff;
    int recbuff;
    int optlen=sizeof(sendbuff);

    dbg("mausb_host_export_device");
    /* only the first interface is true */
    snprintf(attr_path, sizeof(attr_path), "%s/%s:%d.%d/%s",
         edev->udev.path, edev->udev.busid,
         edev->udev.bConfigurationValue, 0, attr_name);

    dbg("open_attribute attr_path %s", attr_path);

    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, &optlen);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recbuff, &optlen);
    dbg("send buff %d\n",sendbuff);
    dbg("rec buff %d\n",recbuff);
    snprintf(sockfd_buff, sizeof(sockfd_buff), "%d\n", sockfd);
    dbg("write: %s", sockfd_buff);

    mausb_write_attr(attr_path,sockfd_buff);

    return ret;
}
