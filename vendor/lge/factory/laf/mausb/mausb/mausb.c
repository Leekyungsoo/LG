/*
 * mausb.c
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


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <syslog.h>

#include "mausb_common.h"
#include "mausb.h"

static int mausb_help(int argc, char *argv[]);

static const char mausb_usage_string[] =
    "mausb [--debug] [--log] [version]\n"
    "             [help] <command> <args>\n";

static void mausb_usage(void)
{
    printf("usage: %s", mausb_usage_string);
}

struct command {
    const char *name;
    int (*fn)(int argc, char *argv[]);
    const char *help;
    void (*usage)(void);
};

static const struct command cmds[] = {
    {
        .name  = "help",
        .fn    = mausb_help,
        .help  = NULL,
        .usage = NULL
    },
    /*{
        .name  = "attach",
        .fn    = mausb_attach,
        .help  = "Attach a remote USB device",
        .usage = mausb_attach_usage
    },
    {
        .name  = "detach",
        .fn    = mausb_detach,
        .help  = "Detach a remote USB device",
        .usage = mausb_detach_usage
    },*/
    /*{
        .name  = "list",
        .fn    = mausb_list,
        .help  = "List exportable or local USB devices",
        .usage = mausb_list_usage
    },*/
    {
        .name  = "bind",
        .fn    = mausb_bind,
        .help  = "Bind device to " MAUSB_HOST_DRV_NAME ".ko",
        .usage = mausb_bind_usage
    },
    {
        .name  = "unbind",
        .fn    = mausb_unbind,
        .help  = "Unbind device from " MAUSB_HOST_DRV_NAME ".ko",
        .usage = mausb_unbind_usage
    },
    { NULL, NULL, NULL, NULL }
};

static int mausb_help(int argc, char *argv[])
{
    const struct command *cmd;
    int i;
    int ret = 0;

    if (argc > 1 && argv++) {
        for (i = 0; cmds[i].name != NULL; i++)
            if (!strcmp(cmds[i].name, argv[0]) && cmds[i].usage) {
                cmds[i].usage();
                goto done;
            }
        ret = -1;
    }

    mausb_usage();
    printf("\n");
    for (cmd = cmds; cmd->name != NULL; cmd++)
        if (cmd->help != NULL)
            printf("  %-10s %s\n", cmd->name, cmd->help);
    printf("\n");
done:
    return ret;
}

static int run_command(const struct command *cmd, int argc, char *argv[])
{
    dbg("running command: `%s'", cmd->name);
    return cmd->fn(argc, argv);
}

int main(int argc, char *argv[])
{
    static const struct option opts[] = {
        { "debug", no_argument, NULL, 'd' },
        { "log",   no_argument, NULL, 'l' },
        { NULL,    0,           NULL,  0  }
    };

    char *cmd;
    int opt;
    int i, rc = -1;

    mausb_use_stderr = 1;
    opterr = 0;
    for (;;) {
        opt = getopt_long(argc, argv, "+d", opts, NULL);

        if (opt == -1)
            break;

        switch (opt) {
        case 'd':
            mausb_use_debug = 1;
            break;
        case 'l':
            mausb_use_syslog = 1;
            openlog("", LOG_PID, LOG_USER);
            break;
        case '?':
            printf("mausb: invalid option\n");
        default:
            mausb_usage();
            goto out;
        }
    }

    cmd = argv[optind];
    if (cmd) {
        for (i = 0; cmds[i].name != NULL; i++)
            if (!strcmp(cmds[i].name, cmd)) {
                argc -= optind;
                argv += optind;
                optind = 0;
                rc = run_command(&cmds[i], argc, argv);
                goto out;
            }
    }

    /* invalid command */
    mausb_help(0, NULL);
out:
    return EXIT_FAILURE;
}
