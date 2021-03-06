#!/bin/ksh -p
#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
#

#
# ID:  xattr_006
#
# DESCRIPTION:
# Verify links between xattr and normal file namespace fail
#
# STRATEGY:
#	1. Create a file and add an xattr to it (to ensure the namespace exists)
#       2. Verify we're unable to create a symbolic link
#	3. Verify we're unable to create a hard link
#

. $STF_SUITE/include/libtest.ksh

tc_id=xattr_006
tc_desc="Verify links between xattr and normal file namespace fail"
print_test_case $tc_id - $tc_desc

if [[ $STC_CIFS_CLIENT_DEBUG == 1 ]] || \
	[[ *:${STC_CIFS_CLIENT_DEBUG}:* == *:$tc_id:* ]]; then
    set -x
fi

server=$(server_name) || return

testdir_init $TDIR
smbmount_clean $TMNT
smbmount_init $TMNT

cmd="mount -F smbfs //$TUSER:$TPASS@$server/public $TMNT"
cti_execute -i '' FAIL $cmd
if [[ $? != 0 ]]; then
	cti_fail "FAIL: smbmount can't mount the public share unexpectedly"
	return
else
	cti_report "PASS: smbmount can mount the public share as expected"
fi

smbmount_getmntopts $TMNT |grep /xattr/ >/dev/null
if [[ $? != 0 ]]; then
	smbmount_clean $TMNT
	cti_unsupported "UNSUPPORTED (no xattr in this mount)"
	return
fi

# Create files set some xattrs on them.

cti_execute_cmd "touch $TMNT/test_file"
create_xattr $TMNT/test_file passwd /etc/passwd

# Try to create a soft link from the xattr namespace to the default namespace

cti_execute_cmd "runat $TMNT/test_file ln -s /etc/passwd foo"
if [[ $? == 0 ]]; then
	cti_fail "FAIL: symbolic link between xattr and normal file namespace succeeded unexpectedly"
	return
else
	cti_report "PASS: symbolic link between xattr and normal file namespace failed as expected"
fi

# Try to create a hard link from the xattr namespace to the default namespace

cti_execute_cmd "runat $TMNT/test_file ln /etc/passwd foo"
if [[ $? == 0 ]]; then
	cti_fail "FAIL: hard link between xattr and normal file namespace succeeded unexpectedly "
	return
else
	cti_report "PASS: hard link between xattr and normal file namespace failed as expected"
fi

smbmount_clean $TMNT
cti_pass "$tc_id: PASS"
