dnl ######################################################################
dnl an M4 macro to include a list of common headers being used everywhere
define(AC_MOUNT_HEADERS,
[
#include "${srcdir}/include/mount_headers.h"

$1
]
)
dnl ======================================================================
