dnl $Id: configure.ac,v 1.19 2013/07/22 13:34:05 setup Exp $
dnl
dnl Copyright (c) 2013
dnl
dnl Written for Thunar by MingWang Xiao <eagle_xmw@aliyun.com>.
dnl

dnl ***************************
dnl *** Version information ***
dnl ***************************
AC_CONFIG_MACRO_DIR([m4])
m4_define([tdp_version_major], [0])
m4_define([tdp_version_minor], [1])
m4_define([tdp_version_micro], [0])
m4_define([tdp_version_build], [r00001])
m4_define([tdp_version_tag], [])
m4_define([tdp_version], [tdp_version_major().tdp_version_minor().tdp_version_micro()ifelse(tdp_version_tag(), [], [], [tdp_version_tag()-tdp_version_build()])])

m4_define([tdp_debug_default], [ifelse(tdp_version_tag(), [svn], [full], [minimum])])

dnl ***************************
dnl *** Initialize autoconf ***
dnl ***************************
AC_COPYRIGHT([Copyright (c) 2013 MingWang Xiao. All rights reserved.
        
Written for Thunar by MingWang Xiao <eagle_xmw@aliyun.com>.])
AC_INIT([thunar-diff-plugin], [tdp_version()], [eagle_xmw@aliyun.com])
AC_PREREQ([2.50])
AC_REVISION([$Id: configure.ac,v 1.19 2013/07/22 13:34:05 setup Exp $])

dnl ***************************
dnl *** Initialize automake ***
dnl ***************************
AM_INIT_AUTOMAKE([1.8 dist-bzip2 tar-ustar no-dist-gzip])
AM_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE()

dnl *******************************
dnl *** Check for UNIX variants ***
dnl *******************************
AC_AIX()
AC_ISC_POSIX()
AC_MINIX()

dnl ********************************
dnl *** Check for basic programs ***
dnl ********************************
AC_PROG_CC()
AC_PROG_LD()
AC_PROG_INSTALL()

dnl **************************
dnl *** Initialize libtool ***
dnl **************************
LT_PREREQ([2.2.6])
LT_INIT([disable-static])
dnl AC_DISABLE_STATIC()
dnl AC_PROG_LIBTOOL()

dnl *****************************
dnl *** default install path  ***
dnl *****************************
AC_PREFIX_DEFAULT(/usr)

dnl ******************************
dnl *** Check for i18n support ***
dnl ******************************
dnl XDT_I18N([zh_CN])

AM_GNU_GETTEXT([external])
GETTEXT_PACKAGE=thunar-diff-plugin
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [GETTEXT thunar-diff-plugin])

dnl gettext and stuff
dnl ALL_LINGUAS="zh_CN"
dnl AM_GLIB_GNU_GETTEXT()

dnl ***********************************
dnl *** Check for required packages ***
dnl ***********************************
dnl XDT_CHECK_PACKAGE([THUNARX], [thunarx-2], [0.4.0])
dnl XDT_CHECK_PACKAGE([THUNAR_VFS], [thunar-vfs-1], [0.4.0])
pkg_modules="thunarx-2 >= 0.4 gtk+-2.0 >= 2.0.0"
PKG_CHECK_MODULES(THUNARX, [$pkg_modules])
AC_SUBST(THUNARX_VERSION)
AC_SUBST(THUNARX_CFLAGS)
AC_SUBST(THUNARX_LIBS)
AC_SUBST(THUNARX_REQUIRED_VERSION)

dnl ***********************************
dnl *** Check for debugging support ***
dnl ***********************************
AC_ARG_ENABLE([debug],
AC_HELP_STRING([--enable-debug=@<:@no/minimum/yes/full@:>@], [Turn on debugging @<:@default=tdp_debug_default@:>@]),
  [], [enable_debug=tdp_debug_default])
AC_MSG_CHECKING([whether to enable debugging support])
if test x"$enable_debug" = x"full" -o x"$enable_debug" = x"yes"; then
  dnl Check whether the compiler accepts the -fno-strict-aliasing
  dnl switch, as certain packages - like Glib - still don't work
  dnl properly with newer compilers.
  AC_MSG_RESULT([$enable_debug])

  save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -fno-strict-aliasing"
  AC_MSG_CHECKING([whether $CC accepts -fno-strict-aliasing])
  AC_COMPILE_IFELSE(AC_LANG_SOURCE([int x;]), [
    AC_MSG_RESULT([yes])
    PLATFORM_CFLAGS="$PLATFORM_CFLAGS -fno-strict-aliasing"
  ], [
    AC_MSG_RESULT([no])
  ])
  CFLAGS="$save_CFLAGS"

  dnl Make sure we detect possible errors
  PLATFORM_CFLAGS="$PLATFORM_CFLAGS -Werror -Wall"

  dnl Paranoia for --enable-debug=full
  if test x"$enable_debug" = x"full"; then
    PLATFORM_CFLAGS="$PLATFORM_CFLAGS -O0 -g3 -DG_ENABLE_DEBUG"
    PLATFORM_CPPFLAGS="$PLATFORM_CPPFLAGS -DG_DISABLE_DEPRECATED -DG_ENABLE_DEBUG"
  fi
else
  dnl Disable object cast checks
  PLATFORM_CPPFLAGS="$PLATFORM_CPPFLAGS -DG_DISABLE_CAST_CHECKS"

  dnl Disable all checks for --enable-debug=no
  if test x"$enable_debug" = x"no"; then
    PLATFORM_CPPFLAGS="$PLATFORM_CPPFLAGS -DG_DISABLE_ASSERT -DG_DISABLE_CHECKS"
  fi
fi

dnl **************************************
dnl *** Check for linker optimizations ***
dnl **************************************
AC_MSG_CHECKING([whether $LD accepts -O1])
case `$LD -O1 -v 2>&1 </dev/null` in
*GNU* | *'with BFD'*)
  PLATFORM_LDFLAGS="$PLATFORM_LDFLAGS -Wl,-O1"
  AC_MSG_RESULT([yes])
  ;;
*)
  AC_MSG_RESULT([no])
  ;;
esac

dnl *********************************
dnl *** Substitute platform flags ***
dnl *********************************
AC_MSG_CHECKING([PLATFORM_CPPFLAGS])
AC_MSG_RESULT([$PLATFORM_CPPFLAGS])
AC_SUBST([PLATFORM_CPPFLAGS])
AC_MSG_CHECKING([PLATFORM_CFLAGS])
AC_MSG_RESULT([$PLATFORM_CFLAGS])
AC_SUBST([PLATFORM_CFLAGS])
AC_MSG_CHECKING([PLATFORM_LDFLAGS])
AC_MSG_RESULT([$PLATFORM_LDFLAGS])
AC_SUBST([PLATFORM_LDFLAGS])

AC_OUTPUT([po/Makefile.in 
Makefile
icons/Makefile
icons/16x16/Makefile
icons/scalable/Makefile
thunar-diff-plugin/Makefile
])

