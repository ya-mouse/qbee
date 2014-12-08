#
# QB Makefile
#
# Copyright (c) 2014 Anton D. Kachalov <mouse@yandex.ru>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
ifeq ($(V),1)
Q      :=
PYQ    :=
else
Q      := @
PYQ    := -q
MAKE   += -s
endif

all:
	@echo " [EXT]  build all"
	python3.4m setup.py build $(PYQ)

install:
	python3.4m setup.py install $(PYQ)

clean:
	python3.4m setup.py clean $(PYQ)
	@rm -rf build
