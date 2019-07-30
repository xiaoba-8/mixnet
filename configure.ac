#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([include/mix/net/net_errno.h])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([mixnet], 1.0)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_MKDIR_P
 
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/param.h sys/socket.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_PROG_LIBTOOL

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([bzero gethostbyname inet_ntoa memset select socket strerror])

AC_CONFIG_FILES([lib/Makefile 
		src/Makefile])
AC_OUTPUT(Makefile)
