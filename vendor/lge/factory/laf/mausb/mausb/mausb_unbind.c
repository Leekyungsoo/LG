/*
 * mausb_unbind.c
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
#include <getopt.h>

#include "mausb_common.h"
#include "utils.h"
#include "mausb.h"

static const char mausb_unbind_usage_string[] =
    "mausb unbind <args>\n"
    "    -b, --busid=<busid>    Unbind " MAUSB_HOST_DRV_NAME ".ko from "
    "device on <busid>\n";

void mausb_unbind_usage(void)
{
    printf("usage: %s", mausb_unbind_usage_string);
}

static int unbind_device(char *busid)
{
    int rc, ret = 0;
    char attr_name[] = "bConfigurationValue";

    /* notify driver of unbind */
    rc = modify_match_busid(busid, 0);
    if (rc < 0) {
        err("unable to unbind device on %s", busid);
    }

    mausb_write_device_attr(attr_name,"1");
    printf("unbind device on busid %s: complete\n", busid);
    return ret;
}

int mausb_unbind(int argc, char *argv[])
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
            ret = unbind_device(optarg);
            goto out;
        default:
            goto err_out;
        }
    }
err_out:
    mausb_unbind_usage();
out:
    return ret;
}
