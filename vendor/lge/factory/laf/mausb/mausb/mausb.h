/*
 * mausb.h
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

#ifndef __MAUSB_H
#define __MAUSB_H

/* mausb commands */
int mausb_attach(int argc, char *argv[]);
int mausb_detach(int argc, char *argv[]);
int mausb_list(int argc, char *argv[]);
int mausb_bind(int argc, char *argv[]);
int mausb_unbind(int argc, char *argv[]);

void mausb_attach_usage(void);
void mausb_detach_usage(void);
void mausb_list_usage(void);
void mausb_bind_usage(void);
void mausb_unbind_usage(void);

#endif /* __MAUSB_H */
