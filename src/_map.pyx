#
# Generic map_interface type
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
import types

class map_getset:
    def __get__(self, obj, klass):
        return [self, obj, klass]

cdef public class map_interface(qb_interface_t) [ object map_interface_st, type map_interface_type ]:
    def __init__(self, obj):
        super().__init__(obj)
        self._obj._map_obj = [self]

    @classmethod
    def register_interface(klass, target):
        target.map = map_getset()

    cpdef int add_map(map_interface self, qb_object_t space, qb_object_t dev, qb_object_t target, int start):
        print(self._obj._map_obj) #, space, dev, target, start)
        return 0
