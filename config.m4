dnl $Id$
dnl config.m4 for extension t2sdk

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(t2sdk, for t2sdk support,
dnl Make sure that the comment is aligned:
dnl [  --with-t2sdk             Include t2sdk support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(t2sdk, whether to enable t2sdk support,
dnl Make sure that the comment is aligned:
 [  --enable-t2sdk           Enable t2sdk support])

if test "$PHP_T2SDK" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-t2sdk -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/t2sdk.h"  # you most likely want to change this
  dnl if test -r $PHP_T2SDK/$SEARCH_FOR; then # path given as parameter
  dnl   T2SDK_DIR=$PHP_T2SDK
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for t2sdk files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       T2SDK_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$T2SDK_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the t2sdk distribution])
  dnl fi

  dnl # --with-t2sdk -> add include path

  dnl PHP_ADD_INCLUDE($T2SDK_DIR/include)

  dnl # --with-t2sdk -> check for lib and symbol presence
  dnl LIBNAME=t2sdk # you may want to change this
  dnl LIBSYMBOL=t2sdk # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $T2SDK_DIR/$PHP_LIBDIR, T2SDK_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_T2SDKLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong t2sdk lib version or lib not found])
  dnl ],[
  dnl   -L$T2SDK_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(T2SDK_SHARED_LIBADD)


	PHP_REQUIRE_CXX()
	PHP_SUBST(T2SDK_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, T2SDK_SHARED_LIBADD)
	PHP_NEW_EXTENSION(t2sdk, t2sdk.cc demo.cc, $ext_shared)
fi
