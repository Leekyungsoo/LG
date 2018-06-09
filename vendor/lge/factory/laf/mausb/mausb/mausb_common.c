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

#include "mausb_common.h"
#include "names.h"

#undef  PROGNAME
#define PROGNAME "libmausb"

int mausb_use_syslog = 1;
int mausb_use_stderr = 1;
int mausb_use_debug  = 1;

struct speed_string {
    int num;
    char *speed;
    char *desc;
};

static const struct speed_string speed_strings[] = {
    { USB_SPEED_UNKNOWN, "unknown", "Unknown Speed"},
    { USB_SPEED_LOW,  "1.5", "Low Speed(1.5Mbps)"  },
    { USB_SPEED_FULL, "12",  "Full Speed(12Mbps)" },
    { USB_SPEED_HIGH, "480", "High Speed(480Mbps)" },
    { 0, NULL, NULL }
};

struct portst_string {
    int num;
    char *desc;
};

static struct portst_string portst_strings[] = {
    { SDEV_ST_AVAILABLE,    "Device Available" },
    { SDEV_ST_USED,        "Device in Use" },
    { SDEV_ST_ERROR,    "Device Error"},
    { VDEV_ST_NULL,        "Port Available"},
    { VDEV_ST_NOTASSIGNED,    "Port Initializing"},
    { VDEV_ST_USED,        "Port in Use"},
    { VDEV_ST_ERROR,    "Port Error"},
    { 0, NULL}
};

const char *mausb_status_string(int32_t status)
{
    for (int i=0; portst_strings[i].desc != NULL; i++)
        if (portst_strings[i].num == status)
            return portst_strings[i].desc;

    return "Unknown Status";
}

const char *mausb_speed_string(int num)
{
    for (int i=0; speed_strings[i].speed != NULL; i++)
        if (speed_strings[i].num == num)
            return speed_strings[i].desc;

    return "Unknown Speed";
}

#define DBG_UDEV_INTEGER(name)\
    dbg("%-20s = %x", to_string(name), (int) udev->name)

#define DBG_UINF_INTEGER(name)\
    dbg("%-20s = %x", to_string(name), (int) uinf->name)

void dump_usb_interface(struct mausb_usb_interface *uinf)
{
    char buff[BUFFER_SIZE] = {0,};
    mausb_names_get_class(buff, sizeof(buff),
            uinf->bInterfaceClass,
            uinf->bInterfaceSubClass,
            uinf->bInterfaceProtocol);
    dbg("%-20s = %s", "Interface(C/SC/P)", buff);
}

void dump_usb_device(struct mausb_usb_device *udev)
{
    char buff[BUFFER_SIZE] = {0,};

    dbg("%-20s = %s", "path",  udev->path);
    dbg("%-20s = %s", "busid", udev->busid);

    mausb_names_get_class(buff, sizeof(buff),
            udev->bDeviceClass,
            udev->bDeviceSubClass,
            udev->bDeviceProtocol);
    dbg("%-20s = %s", "Device(C/SC/P)", buff);

    DBG_UDEV_INTEGER(bcdDevice);

    mausb_names_get_product(buff, sizeof(buff),
            udev->idVendor,
            udev->idProduct);
    dbg("%-20s = %s", "Vendor/Product", buff);

    DBG_UDEV_INTEGER(bNumConfigurations);
    DBG_UDEV_INTEGER(bNumInterfaces);

    dbg("%-20s = %s", "speed", mausb_speed_string(udev->speed));

    DBG_UDEV_INTEGER(busnum);
    DBG_UDEV_INTEGER(devnum);
}

void mausb_get_device_attr(const char *attr_name,const char *attr_value)
{
    int fd;
    int length;
    char attr_path[SYSFS_PATH_MAX] = {0,};
    char lattr_value[ATTR_BUFFER] = {0,};

    dbg("mausb_get_device_attr name %s",attr_name);
    snprintf(attr_path, sizeof(attr_path), "/sys/bus/usb/devices/1-1/%s", attr_name);
    dbg("mausb_get_device_attr path %s",attr_path);

    fd=open(attr_path,O_RDONLY);
    if(fd<0)
    {
        dbg("mausb_get_device_attr file open failed %d",fd);
        return;
    }

    length=read(fd, lattr_value, sizeof(lattr_value));
    dbg("mausb_get_device_attr file length %d",length);
    dbg("mausb_get_device_attr value %s",lattr_value);

    if (length > 0)
        strcpy(attr_value,lattr_value);
    else
        dbg("ERROR: length < 0");

    dbg("mausb_get_device_attr value %s",attr_value);
    close(fd);
}

void mausb_get_driver_attr(const char *attr_name,char *attr_value)
{
    int fd;
    char attr_path[SYSFS_PATH_MAX] = {0,};

    snprintf(attr_path, sizeof(attr_path), "/sys/bus/usb/drivers/mausb-host/%s", attr_name);

    fd=open(attr_path,O_RDONLY);
    read(fd, attr_value, sizeof(attr_value));
}


void mausb_write_device_attr(const char *attr_name,char *attr_value)
{
    int fd;
    int length;
    char attr_path[SYSFS_PATH_MAX] = {0,};

    snprintf(attr_path, sizeof(attr_path), "/sys/bus/usb/devices/1-1/%s", attr_name);

    dbg("mausb_write_device_attr name %s",attr_path);

    fd=open(attr_path,O_WRONLY);

    if(fd<0)
    {
        dbg("mausb_write_device_attr file open failed %d",fd);
        return;
    }

    dbg("mausb_write_device_attr value %s %d",attr_value,strlen(attr_value));

    length=write(fd, attr_value, strlen(attr_value));

    if(length!=strlen(attr_value))
    {
        dbg("mausb_write_device_attr fail %d",length);
    }

    close(fd);
}

void mausb_write_driver_attr(const char *attr_name,const char *attr_value)
{
    int fd;
    int length;
    char attr_path[SYSFS_PATH_MAX] = {0,};

    snprintf(attr_path, sizeof(attr_path), "/sys/bus/usb/drivers/mausb-host/%s", attr_name);

    dbg("mausb_write_driver_attr name %s",attr_path);

    fd = open(attr_path,O_WRONLY);
    if (fd<0)
    {
        dbg("mausb_write_driver_attr file open failed %d",fd);
        return;
    }

    dbg("mausb_write_driver_attr value %s %d",attr_value,strlen(attr_value));

    length = write(fd, attr_value, strlen(attr_value));
    if (length!=strlen(attr_value))
    {
        dbg("mausb_write_driver_attr fail %d",length);
    }

    dbg("mausb_write_driver_attr file length %d",length);
    close(fd);
}

void mausb_write_attr(const char *attr_name,const char *attr_value)
{
    int fd;
    int length;

    dbg("mausb_write_attr name %s",attr_name);

    fd = open(attr_name,O_WRONLY);
    if (fd<0)
    {
        dbg("mausb_write_attr file open failed %d",fd);
        return;
    }
    dbg("mausb_write_attr value %s %d",attr_value,strlen(attr_value));

    length = write(fd, attr_value, strlen(attr_value));
    if (length!=strlen(attr_value))
    {
        dbg("mausb_write_attr fail %d",length);
    }

    dbg("mausb_write_attr file length %d",length);
    close(fd);
}

void mausb_read_attr(const char *attr_name,const char *attr_value)
{
    int fd;
    int length;

    dbg("mausb_read_attr name %s",attr_name);

    fd = open(attr_name,O_RDONLY);
    if (fd<0)
    {
        dbg("mausb_read_attr file open failed %d",fd);
        return;
    }

    length = read(fd, attr_value, sizeof(attr_value));
    dbg("mausb_get_device_attr file length %d",length);
    dbg("mausb_get_device_attr value %s",attr_value);
    close(fd);
}

int read_attr_value(char *sdevpath, const char *name, const char *format)
{
    char attrpath[SYSFS_PATH_MAX] = {0,};
    char attrvalue[ATTR_BUFFER] = {0,};
    int num = 0;
    int ret;

    snprintf(attrpath, sizeof(attrpath), "%s/%s", sdevpath, name);
    dbg("read_attr_value attr_path=%s",attrpath);

    mausb_read_attr(attrpath,attrvalue);

    ret = sscanf(attrvalue, format, &num);
    if (ret < 1) {
        dbg("sscanf failed");
    }
    dbg("read_attr_value num=%d",num);
    return num;
}

int read_attr_speed(char *sdevpath)
{
    char attrpath[SYSFS_PATH_MAX] = {0,};
    char speed[BUFFER_SIZE] = {0,};
    int ret;

    snprintf(attrpath, sizeof(attrpath), "%s/%s", sdevpath, "speed");
    mausb_read_attr(attrpath,speed);

    ret = sscanf(speed, "%s\n", speed);
    if (ret < 1) {
        dbg("sscanf failed");
    }

    for (int i=0; speed_strings[i].speed != NULL; i++) {
        if (!strcmp(speed, speed_strings[i].speed))
            return speed_strings[i].num;
    }
    return USB_SPEED_UNKNOWN;
}

#define READ_ATTR(object, type, dev, name, format)\
    do { (object)->name = (type) read_attr_value(dev, to_string(name), format); } while (0)


int read_usb_device(char *sdevpath, struct mausb_usb_device *udev)
{
    dbg("read_usb_device :%s",sdevpath);

    READ_ATTR(udev, uint8_t,  sdevpath, bDeviceClass,        "%02x\n");
    READ_ATTR(udev, uint8_t,  sdevpath, bDeviceSubClass,    "%02x\n");
    READ_ATTR(udev, uint8_t,  sdevpath, bDeviceProtocol,    "%02x\n");
    READ_ATTR(udev, uint16_t, sdevpath, idVendor,        "%04x\n");
    READ_ATTR(udev, uint16_t, sdevpath, idProduct,        "%04x\n");
    READ_ATTR(udev, uint16_t, sdevpath, bcdDevice,        "%04x\n");
    READ_ATTR(udev, uint8_t,  sdevpath, bConfigurationValue,    "%02x\n");
    READ_ATTR(udev, uint8_t,  sdevpath, bNumConfigurations,    "%02x\n");
    READ_ATTR(udev, uint8_t,  sdevpath, bNumInterfaces,        "%02x\n");
    READ_ATTR(udev, uint8_t,  sdevpath, devnum,            "%d\n");
    udev->speed = read_attr_speed(sdevpath);

    strncpy(udev->path,  sdevpath,  SYSFS_PATH_MAX);
    strncpy(udev->busid, "1-1", SYSFS_BUS_ID_SIZE);
    udev->busnum = 1;
    return 0;
}

int read_usb_interface(struct mausb_usb_device *udev, int i,
               struct mausb_usb_interface *uinf)
{
    char busid[SYSFS_BUS_ID_SIZE] = {0,};
    char infpath[SYSFS_PATH_MAX] = {0,};

    dbg("read_usb_interface ");
    sprintf(infpath, "%s%s:%d.%d", "/sys/bus/usb/devices/",udev->busid, udev->bConfigurationValue, i);
    dbg("read_usb_interface %s ",infpath);

    READ_ATTR(uinf, uint8_t,  infpath, bInterfaceClass,        "%02x\n");
    READ_ATTR(uinf, uint8_t,  infpath, bInterfaceSubClass,    "%02x\n");
    READ_ATTR(uinf, uint8_t,  infpath, bInterfaceProtocol,    "%02x\n");
    return 0;
}

int mausb_names_init(char *f)
{
    return names_init(f);
}

void mausb_names_free()
{
    names_free();
}

void mausb_names_get_product(char *buff, size_t size, uint16_t vendor, uint16_t product)
{
    const char *prod, *vend;

    prod = names_product(vendor, product);
    if (!prod)
        prod = "unknown product";

    vend = names_vendor(vendor);
    if (!vend)
        vend = "unknown vendor";

    snprintf(buff, size, "%s : %s (%04x:%04x)", vend, prod, vendor, product);
}

void mausb_names_get_class(char *buff, size_t size, uint8_t class, uint8_t subclass, uint8_t protocol)
{
    const char *c, *s, *p;

    if (class == 0 && subclass == 0 && protocol == 0) {
        snprintf(buff, size, "(Defined at Interface level) (%02x/%02x/%02x)", class, subclass, protocol);
        return;
    }

    p = names_protocol(class, subclass, protocol);
    if (!p)
        p = "unknown protocol";

    s = names_subclass(class, subclass);
    if (!s)
        s = "unknown subclass";

    c = names_class(class);
    if (!c)
        c = "unknown class";

    snprintf(buff, size, "%s / %s / %s (%02x/%02x/%02x)", c, s, p, class, subclass, protocol);
}
