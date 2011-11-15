/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Yasuo Ohgaki <yohgai@ohgaki.net> <yohgaki@php.net>           |
  +----------------------------------------------------------------------+
*/

/* $Id: header 310447 2011-04-23 21:14:10Z bjori $ */

#ifndef PHP_BIT_H
#define PHP_BIT_H

extern zend_module_entry bit_module_entry;
#define phpext_bit_ptr &bit_module_entry

#ifdef PHP_WIN32
#	define PHP_BIT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_BIT_API __attribute__ ((visibility("default")))
#else
#	define PHP_BIT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(bit);
PHP_MSHUTDOWN_FUNCTION(bit);
PHP_RINIT_FUNCTION(bit);
PHP_RSHUTDOWN_FUNCTION(bit);
PHP_MINFO_FUNCTION(bit);

PHP_FUNCTION(byte_get);
PHP_FUNCTION(byte_set);
PHP_FUNCTION(bit_get);
PHP_FUNCTION(bit_set);

#ifdef ZTS
#define BIT_G(v) TSRMG(bit_globals_id, zend_bit_globals *, v)
#else
#define BIT_G(v) (bit_globals.v)
#endif

#endif	/* PHP_BIT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
