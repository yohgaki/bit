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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "php_bit.h"


/* {{{ bit_functions[]
 *
 * Every user visible function must have an entry in bit_functions[].
 */
const zend_function_entry bit_functions[] = {
	PHP_FE(byte_get,	NULL)		/* get bits from var */
	PHP_FE(byte_set,	NULL)		/* set bits from var. */
	PHP_FE(bit_get,	NULL)		/* get bits from var */
	PHP_FE(bit_set,	NULL)		/* set bits from var. */
	{NULL, NULL, NULL}	/* Must be the last line in bit_functions[] */
};
/* }}} */

/* {{{ bit_module_entry
 */
zend_module_entry bit_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"bit",
	bit_functions,
	PHP_MINIT(bit),
	PHP_MSHUTDOWN(bit),
	NULL,
	NULL,
	PHP_MINFO(bit),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", 
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BIT
ZEND_GET_MODULE(bit)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(bit)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(bit)
{
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(bit)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "bit support", "enabled");
	php_info_print_table_end();
}
/* }}} */

static unsigned char hexchars[] = "0123456789abcdef";

/* {{{ proto string get_bits(string arg)
   Return hex string from binary string*/
PHP_FUNCTION(byte_get)
{
	char *arg = NULL;
	int arg_len;
    smart_str s = {0};
    const unsigned char *p;
    unsigned char c, buf[2];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

    p = (unsigned char *)arg;
    while(arg_len > 0) {
        c = *p++;
        buf[0] = hexchars[c >> 4];
		buf[1] = hexchars[c & 15];
        smart_str_appendl(&s,buf,2);
        arg_len--;
    }
    smart_str_0(&s);
    
    if (s.len) {
        RETURN_STRINGL(s.c, s.len, 0);
    } else {
        RETURN_NULL();
    }
}
/* }}} */

/* {{{ bit_htoi
 */
static int bit_htoi(unsigned char *s)
{
	int value;
	int c;

	c = ((unsigned char *)s)[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = ((unsigned char *)s)[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (value);
}
/* }}} */


/* {{{ proto string set_bits(string arg)
   Return binary string from a hex string */
PHP_FUNCTION(byte_set)
{
	char *arg = NULL;
	int arg_len, left;
    smart_str s = {0};
    const unsigned char *p;
    unsigned char c, buf[3];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

    buf[2] = '\0';
    for (p=(unsigned char *)arg, left=arg_len; left>0; left -= 2) {
        if (left < 0) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Input string is not aligned by 2.");
            break;
        }
        buf[0] = *p++;
        buf[1] = *p++;
        c = (unsigned char)bit_htoi(buf);
        smart_str_appendc(&s, c);
    }
    smart_str_0(&s);
    
    if (s.len) {
        RETURN_STRINGL(s.c, s.len, 0);
    } else {
        RETURN_NULL();
    }
}
/* }}} */


/* {{{ proto string get_bits(string arg)
   Return 0 and 1 bit string from binary string */
PHP_FUNCTION(bit_get)
{
	char *arg = NULL;
	int arg_len, i;
    smart_str s = {0};
    const unsigned char *p;
    unsigned char c;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

    p = (unsigned char *)arg;
    while(*p) {
        c = *p++;
        for (i = 7; i >=0; i--) {
            if ((c>>i) & 0x1) {
                smart_str_appendc(&s,'1');
            } else {
                smart_str_appendc(&s, '0');
            }
        }
    }
    smart_str_0(&s);

    if (s.len) {
        RETURN_STRINGL(s.c, s.len, 0);
    } else {
        RETURN_NULL();
    }
}
/* }}} */


/* {{{ proto string set_bits(string arg)
   Return binary string from a bit string */
PHP_FUNCTION(bit_set)
{
	char *arg = NULL;
	int arg_len, left, i;
    smart_str s = {0};
    const unsigned char *p;
    unsigned char c;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

    for (p=(unsigned char *)arg, left=arg_len; left>0; left -= 8) {
        if (left < 0) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Input string is not aligned by 8.");
            break;
        }
        c = 0;
        for (i=7; i >= 0; i--, p++) {
            if (*p == '1') {
                c |= 0x1 << i;
            } else if (*p != '0') {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "Bit string must be consits of 0 and 1.");
                break;
            }
        }
        smart_str_appendc(&s, c);
    }
    smart_str_0(&s);
    
    if (s.len) {
        RETURN_STRINGL(s.c, s.len, 0);
    } else {
        RETURN_NULL();
    }
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
