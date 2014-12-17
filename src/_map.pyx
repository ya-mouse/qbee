import types
from qb._core import qb_object_t, qb_interface_t

class map_getset:
    def __get__(self, obj, klass):
        return []

cdef class map_interface(qb_interface_t):
    @classmethod
    def register_interface(klass, target):
        target.map = map_getset()

    def add_map(self, space, dev, target, start):
        print(self._obj, space, dev, target, start)
        return 0
