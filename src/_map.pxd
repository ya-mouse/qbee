#
# Generic map_interface's definition file
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
from _core cimport qb_object_t, qb_interface_t

cdef public class map_interface(qb_interface_t) [ object map_interface_st, type map_interface_type ]:
    cpdef int add_map(map_interface self, qb_object_t space, qb_object_t dev, qb_object_t target, int start)
