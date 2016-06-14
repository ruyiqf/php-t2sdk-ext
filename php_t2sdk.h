/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_T2SDK_H
#define PHP_T2SDK_H

#define PHP_T2SDK_EXTNAME  "t2sdk"
#define PHP_T2SDK_EXTVER   "1.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 

extern "C" {
#include "php.h"
}

extern zend_module_entry t2sdk_module_entry;
#define phpext_t2sdk_ptr &t2sdk_module_entry

#define PHP_T2SDK_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
# define PHP_T2SDK_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_T2SDK_API __attribute__ ((visibility("default")))
#else
# define PHP_T2SDK_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "source/t2connection.h"
#include <string.h>

PHP_MINIT_FUNCTION(t2sdk);
PHP_MSHUTDOWN_FUNCTION(t2sdk);
PHP_RINIT_FUNCTION(t2sdk);
PHP_RSHUTDOWN_FUNCTION(t2sdk);
PHP_MINFO_FUNCTION(t2sdk);

PHP_FUNCTION(confirm_t2sdk_compiled); /* For testing, remove later. */

/* 
    Declare any global variables you may need between the BEGIN
  and END macros here:     
*/
ZEND_BEGIN_MODULE_GLOBALS(t2sdk)

CConnectionInterface *g_pConnection;
CConnectionInterface *g_pConnectionHq;

ZEND_END_MODULE_GLOBALS(t2sdk)


/* In every utility function you add that needs to use variables 
   in php_t2sdk_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as T2SDK_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define T2SDK_G(v) TSRMG(t2sdk_globals_id, zend_t2sdk_globals *, v)
#else
#define T2SDK_G(v) (t2sdk_globals.v)
#endif

#endif  /* PHP_T2SDK_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
