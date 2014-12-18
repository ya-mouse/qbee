import types
from _core cimport qb_interface_t, qb_object_t

class map_getset:
    def __get__(self, obj, klass):
        return [self, obj, klass]

cdef public class map_interface(qb_interface_t) [ object map_interface_st, type map_interface_type ]:
    @classmethod
    def register_interface(klass, target):
        target.map = map_getset()

    cpdef int add_map(map_interface self, qb_object_t space, qb_object_t dev, qb_object_t target, int start):
        print(self._obj, space, dev, target, start)
        return 0
