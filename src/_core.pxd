cdef class ClassPropertyDescriptor:
    cdef object fget

cdef public class qb_interface_t [ object qb_interface_st, type qb_interface_type ]:
    cdef public object _obj

cdef public class qb_object_t(object) [ object ob_object_st, type qb_object_type ]:
    cdef public object _iface
    cdef public object _parent
