#! /bin/sh
# a quick hack script to generate necessary files from 
# auto* tools.
#
# WARNING: if you run this you will change the versions
# of the tools which are used and, maybe, required!
touch Makefile.am configure.ac

OS=`uname`
if [ "x$OS" == "xDarwin" ]; then
    CHECK_DIR=/opt/local/share/aclocal
    LIBTOOLIZE=glibtoolize
else
    LIBTOOLIZE=libtoolize
fi

{
    echo "running libtoolize" >&2
    $LIBTOOLIZE --copy --automake
} && {
    echo "running aclocal" >&2
    if [ -n "$CHECK_DIR" ]; then
	aclocal -I m4 -I $CHECK_DIR
    else
	aclocal -I m4
    fi
} && {
    echo "running autoheader [ignore the warnings]" >&2
    autoheader
} && {
    echo "running automake" >&2
    automake --foreign -a -c --add-missing
} && {
    echo "running autoconf" >&2
    autoconf
} && echo "autogen complete" >&2 || {
    echo "ERROR: autogen.sh failed, autogen is incomplete" >&2
    exit 1
}
