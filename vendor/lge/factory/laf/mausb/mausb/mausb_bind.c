/*
 * mausb_bind.c
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
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "mausb_common.h"
#include "utils.h"
#include "mausb.h"

enum unbind_status {
    UNBIND_ST_OK,
    UNBIND_ST_MAUSB_HOST,
    UNBIND_ST_FAILED
};

static const char mausb_bind_usage_string[] =
    "mausb bind <args>\n"
    "    -b, --busid=<busid>    Bind " MAUSB_HOST_DRV_NAME ".ko to device "
    "on <busid>\n";

void mausb_bind_usage(void)
{
    printf("usage: %s", mausb_bind_usage_string);
}

/* call at unbound state */
static int bind_mausb(char *busid)
{
    char attr_name[] = "bind";
    char bNumIntfs[NUM_INF_BUFFER] = {0,};
    char intf_busid[SYSFS_BUS_ID_SIZE] = {0,};
    int i= 0;
    int ret =0;

    mausb_get_device_attr("bNumInterfaces",bNumIntfs);

    dbg("bNumInterfaces %s int%d",bNumIntfs,atoi(bNumIntfs));

    for (i = 0; i < atoi(bNumIntfs); i++) {
        snprintf(intf_busid, SYSFS_BUS_ID_SIZE, "%s:%.1s.%d", busid,
             "1", i);
        mausb_write_driver_attr(attr_name,intf_busid);
    }
    return ret;
}

/* buggy driver may cause dead lock */
static int unbind_other(char *busid)
{
    char intf_busid[SYSFS_BUS_ID_SIZE] = {0,};
    char bConfValue[CONF_VALUE_BUFFER] = {0,};
    char bNumIntfs[NUM_INF_BUFFER] ={0,};
    int i = 0;
    enum unbind_status status = UNBIND_ST_OK;

    mausb_get_device_attr("bConfigurationValue",bConfValue);
    mausb_get_device_attr("bNumInterfaces",bNumIntfs);
    dbg("bConfigurationValue %s",bConfValue);
    dbg("bNumInterfaces %s int%d",bNumIntfs,atoi(bNumIntfs));

    for (i = 0; i < atoi(bNumIntfs); i++) {
        snprintf(intf_busid, SYSFS_BUS_ID_SIZE, "%s:%.1s.%d", busid, "1", i);
        //mausb_write_driver_attr("unbind",intf_busid);
    }
    return status;
}

static int bind_device(char *busid)
{
    int rc;

    rc = unbind_other(busid);
    if (rc == UNBIND_ST_FAILED) {
        dbg("could not unbind driver from device on busid %s", busid);
        return -1;
    } else if (rc == UNBIND_ST_MAUSB_HOST) {
        dbg("device on busid %s is already bound to %s", busid,
            MAUSB_HOST_DRV_NAME);
        return -1;
    }

    rc = modify_match_busid(busid, 1);
    if (rc < 0) {
        dbg("unable to bind device on %s", busid);
        return -1;
    }

    rc = bind_mausb(busid);
    if (rc < 0) {
        dbg("could not bind device to %s", MAUSB_HOST_DRV_NAME);
        modify_match_busid(busid, 0);
        return -1;
    }

    dbg("bind device on busid %s: complete\n", busid);

    return 0;
}

int mausb_bind(int argc, char *argv[])
{
    static const struct option opts[] = {
        { "busid", required_argument, NULL, 'b' },
        { NULL,    0,                 NULL,  0  }
    };

    int opt;
    int ret = -1;

    for (;;) {
        opt = getopt_long(argc, argv, "b:", opts, NULL);

        if (opt == -1)
            break;

        switch (opt) {
        case 'b':
            ret = bind_device(optarg);
            goto out;
        default:
            goto err_out;
        }
    }

err_out:
    mausb_bind_usage();
out:
    return ret;
}
