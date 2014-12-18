#!/usr/bin/python3.4dm
#
# QBee main executable
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
import sys
import platform

qb_system=platform.system().lower()
qb_machine=platform.machine()
qb_pyversion=platform.python_version().rsplit('.',1)[0]
sys.path.append('build/lib.{}-{}-{}-pydebug'.format(qb_system, qb_machine, qb_pyversion))

import qb

qb.main()
