# _core.pyx
# cython: language_level=3
#
# Core types
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
from cpython cimport PyObject, Py_INCREF

cdef extern from "Python.h":
    ctypedef struct PyVarObject:
        PyObject ob_base
    ctypedef struct PyTypeObject:
        PyVarObject ob_base

cdef void _install_metaclass(PyTypeObject *ctype, object metaclass):
    """Installs the metaclass to the object, which is otherwise not
    supported for cdef classes by cython."""
    #cdef PyObject *obj = <PyObject *>ctype
    Py_INCREF(metaclass)
    ctype.ob_base.ob_base.ob_type = <PyTypeObject*>metaclass

cdef class ClassPropertyDescriptor:
    def __init__(self, fget):
        self.fget = fget

    def __get__(self, obj, klass):
        if klass is None:
            klass = type(obj)
        return self.fget.__get__(obj, klass)()

cdef api class qb_interface_t [ object qb_interface_st, type qb_interface_type ]:
    def __cinit__(self, obj):
        if not hasattr(self, '__iface__'):
            setattr(self, '__iface__', None)

    def __init__(self, obj):
        self._obj = obj

    def __repr__(self):
        return '<{} object {} for {}>'.format(type(self).__name__, self.__iface__, self._obj)

    @classmethod
    def register_interface(klass, target):
        pass

class qb_interface_classproperty_t:
    _klass = None
    _ifacedefs = {}

    def __init__(self, klass, ifacedefs):
        self._klass = klass
        self._ifacedefs = ifacedefs

    def __repr__(self):
        return '<qb_interface_classproperty, ifaces={}>'.format([k for k,v in self._ifacedefs.items()])

    @property
    def ifacedefs(self):
        return self._ifacedefs

    def __getattr__(self, attr):
        if attr == '__call__':
             return super().__getattr__(self, attr)
        return self._ifacedefs[attr]

    def __setattr__(self, attr, value):
        builtin = attr[0] == '_'
        if not builtin and issubclass(value, qb_interface_t):
            self._ifacedefs[attr] = value
            value.register_interface(self._klass)
            setattr(value, '__iface__', attr)
            super().__setattr__(attr, value)
        if builtin:
            super().__setattr__(attr, value)

class qb_interface_set_t:
    def __init__(self, obj, ifacedefs):
        self._obj = obj
        self._ifacedefs = {}
        for k,v in ifacedefs.items():
            v = v(obj)
            self.__setattr__(k, v)
            self._ifacedefs[k] = v

    def __repr__(self):
        return '<qb_interface_set, ifaces={}>'.format([k for k,v in self._ifacedefs.items()])


def iface_method(klass):
        # For the first call instantiate ifacedefs
        if klass._iface is None and klass is not qb_object_t:
            klass._iface = qb_interface_classproperty_t(klass, {})

        # For subclass re-instantiate with previously selected ifacedefs
        if klass._iface is not None and klass._iface._klass != klass:
            klass._iface = qb_interface_classproperty_t(klass, klass._iface.ifacedefs)

        return klass._iface

cdef class _qb_interface_setter(type):
    def __init__(cls, name, bases, nmspc):
        cls._iface = None
        cls.iface = ClassPropertyDescriptor(classmethod(iface_method))
        for k,v in nmspc.items():
            if not isinstance(v, type) or not issubclass(v, qb_interface_t) or not hasattr(v, '__iface__'):
                continue
            setattr(cls.iface, v.__iface__, v)
        super(_qb_interface_setter, cls).__init__(name, bases, nmspc)

class qb_object_set_t:
    def __init__(self, parent):
        self._parent = parent
        self._objs = {}

    def __setattr__(self, key, value):
        builtin = key[0] == '_'
        if builtin:
            super().__setattr__(key, value)
            return
        if not isinstance(value, qb_object_t):
            raise TypeError('value `{}\' have to be subclass of `qb_object_t\''.format(value))
        self._objs[key] = value
        value._name = key
        value._parent = self._parent
        super().__setattr__(key, value)

cdef api class qb_object_t(object) [ object ob_object_st, type qb_object_type ]:
    def __cinit__(self):
        self._objs = None
        self._parent = None
        self._name = None

    def __init__(self, *args, **kwargs):
        try:
            self._name = args[0]
        except IndexError:
            pass
        # FIXME: type's field has values while instance don't
        if type(self)._iface is not None and isinstance(type(self)._iface, qb_interface_classproperty_t):
            self.iface = qb_interface_set_t(self, type(self)._iface.ifacedefs)
        self._objs = qb_object_set_t(self)

    def __repr__(self):
        return '<{} object, name={}>'.format(type(self).__name__, self._name)

    @property
    def o(self):
        return self._objs

cdef class qb_root(qb_object_t):
    pass

cdef extern from "Python.h": # hack to force type to be known
    cdef PyTypeObject qb_object_type # hack to install metaclass
_install_metaclass(&qb_object_type, _qb_interface_setter)

root = qb_root('root')
