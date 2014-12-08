#include <qb/qb.h>
#include <qb/core/memoryregion.h>

#include <structmember.h>

static PyObject *
MemoryRegion_name(MemoryRegion* self)
{
    return PyUnicode_FromFormat("%S", "test");
}

static PyObject *get_map(MemoryRegion *self, void *closure)
{
    return self->map;
}

static int set_map(MemoryRegion *self, PyObject *value, void *closure)
{
    fprintf(stderr, "SET MAP\n");
    self->map = value;
    return 0;
}


static PyMethodDef MemoryRegion_methods[] = {
    {"name", (PyCFunction)MemoryRegion_name, METH_NOARGS, "name method"},
    {NULL}
};

static PyMemberDef MemoryRegion_members[] = {
    {NULL}
};

static PyGetSetDef MemoryRegion_getset[] = {
    {"map", (getter)get_map, (setter)set_map, NULL},
    {NULL}
};

static PyTypeObject MemoryRegion_Type = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_memoryregion",
    .tp_basicsize = sizeof(MemoryRegion),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = PyDoc_STR("MemoryRegion"),
    .tp_methods = MemoryRegion_methods,
    .tp_members = MemoryRegion_members,
    .tp_getset = MemoryRegion_getset,
    .tp_new = PyType_GenericNew,
};


/* Module initialization. */

static struct PyModuleDef _memoryregion_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "_memoryregion",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit__memoryregion(void)
{
    PyObject *m, *d;

    qb_object_IMPORT;

    MemoryRegion_Type.tp_base = qb_object_API->interface_type;

    if (PyType_Ready(&MemoryRegion_Type) < 0)
        return NULL;

    m = PyModule_Create(&_memoryregion_module);
    if (m == NULL)
        return NULL;

    /* Add some symbolic constants to the module */
    d = PyModule_GetDict(m);
    if (d == NULL)
        return NULL;

    Py_INCREF(&MemoryRegion_Type);
    PyModule_AddObject(m, "MemoryRegion",
                       (PyObject *)&MemoryRegion_Type);

    return m;
}
