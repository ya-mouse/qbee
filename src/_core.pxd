#
# Core types' definition file
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
cdef class ClassPropertyDescriptor:
    cdef object fget

cdef api class qb_interface_t [ object qb_interface_st, type qb_interface_type ]:
    cdef public object _obj

cdef api class qb_object_t(object) [ object ob_object_st, type qb_object_type ]:
    cdef public object _parent
    cdef public object _name
    cdef public object _iface
    cdef public object _objs
