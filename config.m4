dnl $Id$
dnl config.m4 for extension bit

PHP_ARG_WITH(bit, for bit support,
[  --with-bit             Include bit support])

if test "$PHP_BIT" != "no"; then
  PHP_SUBST(BIT_SHARED_LIBADD)
  PHP_NEW_EXTENSION(bit, bit.c, $ext_shared)
fi
