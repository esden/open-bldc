# libgovernor - Open-BLDC configuration and debug protocol library
# Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

LCOV_OUTPUT  = $(top_srcdir)/doc/lg_test_coverage
LCOV_LCOV    = @LCOV@
LCOV_GENHTML = @GENHTML@
LCOV_GCOV_ENABLED = @GCOV_ENABLED@

SUBDIRS = include src tests doc

# Doxygen
#include $(top_srcdir)/aminclude.am

CLEANFILES = \
*/*.gcda \
*/*.gcdo \
*/*.gcno \
libgovernor.info

MAINTAINERCLEANFILES = \
aclocal.m4 \
configure \
include/config.h.in \
Makefile.in \
config.log \
config.guess \
config.sub \
include/stamp-h \
include/stamp-h.in \
depcomp \
install-sh \
ltmain.sh \
missing

# Make sure no sticky bits go into the distribution.
# (Debian policy is to create user directories in g+s,
# we don't want to export this :o)).
dist-hook:
	-chmod -R a-s $(distdir)

macx_embed: all
	cp src/.libs/libgovernor.dylib .
	install_name_tool -id @executable_path/../Frameworks/libgovernor.dylib libgovernor.dylib

lcov: check
	@if [ "x$(LCOV_GCOV_ENABLED)" = "xyes" ]; then \
		$(LCOV_LCOV) --directory . --capture --output-file libgovernor.info --test-name LG_LCOV; \
		LANG=C $(LCOV_GENHTML) --prefix $(shell pwd | sed s,\/libgovernor,,) --output-directory $(LCOV_OUTPUT) --title "libgovernor Code Coverage" --show-details libgovernor.info; \
		echo "Point a web browser at $(LCOV_OUTPUT)/index.html to see results."; \
	else \
		echo "GCOV is not enabled. Please run './configure --enable-gcov; make clean'"; \
	fi

lcov-clean:
	$(RM) libgovernor.info
	$(RM) -r $(LCOV_OUTPUT)

doxygen-clean:
	$(RM) -r -f doc/doxygen

