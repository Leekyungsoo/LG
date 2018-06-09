/*
 * Copyright (c) 2008, 2009, Oracle and/or its affiliates. All rights reserved.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */
// AUTOMATICALLY GENERATED FILE - DO NOT EDIT
package sun.nio.fs;

import android.system.OsConstants;

import java.io.ObjectStreamClass;

class UnixConstants {

    private UnixConstants() {
    }

    // Android-changed (http://b/32203242): Instead of maintaining a separate set of UnixConstants,
    // use the values from android.system.OsConstants.  AT_SYMLINK_NOFOLLOW is used by fstatat when
    // a symbolic link is passed. With AT_SYMLINK_NOLLOW, it doesn't dereference and instead return
    // information about the link, and with it, returns the information about the target. There is
    // no suitable alternative for the flag in android.system.OsConstant, therefore, left untouched.
    // With AT_REMOVEDIR, unlinkat works equivalent to rmdir. Again there is no suitable alternative
    // for the flag in android.system.OsConstant, therefore, left unchanged.
    static final int O_RDONLY = OsConstants.O_RDONLY;

    static final int O_WRONLY = OsConstants.O_WRONLY;

    static final int O_RDWR = OsConstants.O_RDWR;

    static final int O_APPEND = OsConstants.O_APPEND;

    static final int O_CREAT = OsConstants.O_CREAT;

    static final int O_EXCL = OsConstants.O_EXCL;

    static final int O_TRUNC = OsConstants.O_TRUNC;

    static final int O_SYNC = OsConstants.O_SYNC;

    static final int O_DSYNC = OsConstants.O_DSYNC;

    static final int O_NOFOLLOW = OsConstants.O_NOFOLLOW;

    static final int S_IAMB = get_S_IAMB();

    static final int S_IRUSR = OsConstants.S_IRUSR;

    static final int S_IWUSR = OsConstants.S_IWUSR;

    static final int S_IXUSR = OsConstants.S_IXUSR;

    static final int S_IRGRP = OsConstants.S_IRGRP;

    static final int S_IWGRP = OsConstants.S_IWGRP;

    static final int S_IXGRP = OsConstants.S_IXGRP;

    static final int S_IROTH = OsConstants.S_IROTH;

    static final int S_IWOTH = OsConstants.S_IWOTH;

    static final int S_IXOTH = OsConstants.S_IXOTH;

    static final int S_IFMT = OsConstants.S_IFMT;

    static final int S_IFREG = OsConstants.S_IFREG;

    static final int S_IFDIR = OsConstants.S_IFDIR;

    static final int S_IFLNK = OsConstants.S_IFLNK;

    static final int S_IFCHR = OsConstants.S_IFCHR;

    static final int S_IFBLK = OsConstants.S_IFBLK;

    static final int S_IFIFO = OsConstants.S_IFIFO;

    static final int R_OK = OsConstants.R_OK;

    static final int W_OK = OsConstants.W_OK;

    static final int X_OK = OsConstants.X_OK;

    static final int F_OK = OsConstants.F_OK;

    static final int ENOENT = OsConstants.ENOENT;

    static final int EACCES = OsConstants.EACCES;

    static final int EEXIST = OsConstants.EEXIST;

    static final int ENOTDIR = OsConstants.ENOTDIR;

    static final int EINVAL = OsConstants.EINVAL;

    static final int EXDEV = OsConstants.EXDEV;

    static final int EISDIR = OsConstants.EISDIR;

    static final int ENOTEMPTY = OsConstants.ENOTEMPTY;

    static final int ENOSPC = OsConstants.ENOSPC;

    static final int EAGAIN = OsConstants.EAGAIN;

    static final int ENOSYS = OsConstants.ENOSYS;

    static final int ELOOP = OsConstants.ELOOP;

    static final int EROFS = OsConstants.EROFS;

    static final int ENODATA = OsConstants.ENODATA;

    static final int ERANGE = OsConstants.ERANGE;

    static final int EMFILE = OsConstants.EMFILE;

    static final int AT_SYMLINK_NOFOLLOW = 0x100;

    static final int AT_REMOVEDIR = 0x200;

    // S_IAMB are access mode bits, therefore, calculated by taking OR of all the read, write and
    // execute permissions bits for owner, group and other.
    private static int get_S_IAMB() {
        return (OsConstants.S_IRUSR | OsConstants.S_IWUSR | OsConstants.S_IXUSR |
                OsConstants.S_IRGRP | OsConstants.S_IWGRP | OsConstants.S_IXGRP |
                OsConstants.S_IROTH | OsConstants.S_IWOTH | OsConstants.S_IXOTH);
    }
}
