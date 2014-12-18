from _core cimport qb_object_t, qb_interface_t

cdef public class map_interface(qb_interface_t) [ object map_interface_st, type map_interface_type ]:
    cpdef int add_map(map_interface self, qb_object_t space, qb_object_t dev, qb_object_t target, int start)
