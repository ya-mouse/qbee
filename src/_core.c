#include <qb/qb.h>

#include <structmember.h>

typedef struct {
    PyObject_HEAD
    PyObject *objset;
    PyObject *ifaceset;
    PyObject *dict;
} qb_object_t;

typedef struct {
    PyObject_HEAD
    PyObject *parentobj;
    PyObject *dict;
    uint32_t has_fastcalls;
} qb_interface_t;

typedef struct {
    PyObject_HEAD
    PyObject *o;
} qb_object_set_t;

typedef struct {
    PyObject_HEAD
    PyObject *parentobj;
    PyObject *iface;
} qb_interface_set_t;

PyTypeObject qb_object_type;
PyTypeObject qb_object_set_type;
PyTypeObject qb_interface_type;
PyTypeObject qb_interface_set_type;

PyObject *
qb_object_new(PyTypeObject *to, char *name)
{
    PyObject *obj = PyObject_CallObject((PyObject *) to, NULL);

    return obj;
}

PyObject *
qb_interface_get(qb_object_t *obj, char *name)
{
    // TODO: double check `obj' type
    return PyObject_GetAttrString(obj->ifaceset, name);
}

static int
_Py_object_init(qb_object_t *self, PyObject *args, PyObject *kwds)
{
    PyObject *tuple = PyTuple_Pack(1, self);
//    self->ob_dict = PyDict_New();
    self->objset = PyObject_CallObject((PyObject *) &qb_object_set_type, NULL); //(PyObject *)self);
    self->ifaceset = PyObject_CallObject((PyObject *) &qb_interface_set_type, tuple); // (PyObject *)self);
    Py_DECREF(tuple);
//    Py_INCREF(self->ob_dict);
//    Py_INCREF(self->objset);
//    Py_INCREF(self->ifaceset);
//    fprintf(stderr, "ob=%d\n", Py_TYPE(self)->tp_dictoffset);
    return 0;
}

static int
_Py_object_traverse(qb_object_t *self, visitproc visit, void *arg)
{
    Py_VISIT(self->dict);
    Py_VISIT(self->objset);
    Py_VISIT(self->ifaceset);
    return 0;
}

static int
_Py_interface_init(qb_interface_t *self, PyObject *args, PyObject *kwds)
{
    // TODO: check for arguments (type have to be `qb_object_t')
    self->parentobj = PyTuple_GetItem(args, 0);
    return 0;
}

static int
_Py_interface_traverse(qb_interface_t *self, visitproc visit, void *arg)
{
    Py_VISIT(self->dict);
    Py_VISIT(self->parentobj);
    return 0;
}

static int
_Py_interface_set_init(qb_interface_set_t *self, PyObject *args, PyObject *kwds)
{
    self->parentobj = PyTuple_GetItem(args, 0);
//    self->iface = PyDict_New();
//    Py_INCREF(self->iface);
    return 0;
}

static int
_Py_interface_set_setattr(qb_interface_set_t *self, PyObject *name, PyObject *iface)
{
    PyObject *obj;

    if (PyType_Check(iface)) {
        if (!PyObject_IsSubclass(iface, (PyObject *)&qb_interface_type)) {
            PyErr_SetString(PyExc_TypeError, "class should be subclass of `qb_interface_t'");
            return -1;
        }
    } else {
#if 0
        if (!PyObject_IsInstance(iface, (PyObject *)&qb_interface_type)) {
            PyErr_SetString(PyExc_TypeError, "object should be instance of `qb_interface_t'");
            return -1;
        }
#else
        PyErr_SetString(PyExc_TypeError, "value must be class (subclass of `qb_interface_t')");
        return -1;
#endif
    }
    obj = PyObject_CallObject(iface, PyTuple_Pack(1, self->parentobj));

    return PyObject_GenericSetAttr((PyObject *)self, name, obj);
}

static int
_Py_object_set_init(qb_object_set_t *self, PyObject *args, PyObject *kwds)
{
//    fprintf(stderr, "objset=%p\n", args);
//    self->o = PyDict_New();
//    Py_INCREF(self->o);
    return 0;
}

static int
_Py_object_set_setattr(qb_object_set_t *self, PyObject *name, PyObject *item)
{
    PyObject_GenericSetAttr((PyObject *)self, name, item);
    return 0;
}

static PyMemberDef _Py_object_members[] = {
    {"iface", T_OBJECT, offsetof(qb_object_t, ifaceset), READONLY},
    {"o", T_OBJECT, offsetof(qb_object_t, objset), READONLY},
    {NULL}
};

/*
static PyMemberDef _Py_interface_members[] = {
    {"parent", T_OBJECT, offsetof(qb_interface_t, parentobj), READONLY},
    {NULL}
};

static PyMethodDef _Py_interface_set_methods[] = {
    {"register", (PyCFunction)_Py_interface_set_register, METH_VARARGS,
        PyDoc_STR("register interface")},
    {NULL}
};
*/

PyTypeObject qb_object_type = {
    .ob_base      = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "qb_object_t",
    .tp_basicsize = sizeof(qb_object_t),
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc       = PyDoc_STR("qb_object_t"),
//    .tp_methods   = _Py_interface_set_methods,
    .tp_traverse  = (traverseproc)_Py_object_traverse,
    .tp_members   = _Py_object_members,
    .tp_dictoffset = offsetof(qb_object_t, dict),
    .tp_init      = (initproc)_Py_object_init,
    .tp_new       = PyType_GenericNew,
};

PyTypeObject qb_interface_type = {
    .ob_base      = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "qb_interface_t",
    .tp_basicsize = sizeof(qb_interface_t),
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc       = PyDoc_STR("qb_interface_t"),
    .tp_traverse  = (traverseproc)_Py_interface_traverse,
//    .tp_members   = _Py_interface_members,
    .tp_dictoffset = offsetof(qb_interface_t, dict),
    .tp_init      = (initproc)_Py_interface_init,
    .tp_new       = PyType_GenericNew,
};

PyTypeObject qb_object_set_type = {
    .ob_base      = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "qb_object_set_t",
    .tp_basicsize = sizeof(qb_object_set_t),
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_doc       = PyDoc_STR("qb_object_set_t"),
    .tp_setattro  = (setattrofunc)_Py_object_set_setattr,
    .tp_dictoffset = offsetof(qb_object_set_t, o),
    .tp_init      = (initproc)_Py_object_set_init,
    .tp_new       = PyType_GenericNew,
};

PyTypeObject qb_interface_set_type = {
    .ob_base      = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "qb_interface_set_t",
    .tp_basicsize = sizeof(qb_interface_set_t),
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_doc       = PyDoc_STR("qb_inetrace_set_t"),
    .tp_setattro  = (setattrofunc)_Py_interface_set_setattr,
    .tp_dictoffset = offsetof(qb_interface_set_t, iface),
    .tp_init      = (initproc)_Py_interface_set_init,
    .tp_new       = PyType_GenericNew,
};

/* C API.
 */
static qb_object_CAPI CAPI = {
    &qb_object_type,
    &qb_interface_type,
};

static struct PyModuleDef _core_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "_core",
    .m_size = -1,
};

PyObject *
PyInit__core(void)
{
    PyObject *m, *d, *x;

    if (PyType_Ready(&qb_interface_set_type) < 0)
        return NULL;

    if (PyType_Ready(&qb_object_set_type) < 0)
        return NULL;

    if (PyType_Ready(&qb_interface_type) < 0)
        return NULL;

    if (PyType_Ready(&qb_object_type) < 0)
        return NULL;

    m = PyModule_Create(&_core_module);
    if (m == NULL)
        return NULL;

    /* Add some symbolic constants to the module */
    d = PyModule_GetDict(m);
    if (d == NULL)
        return NULL;

    Py_INCREF(&qb_interface_set_type);
    Py_INCREF(&qb_object_set_type);
    Py_INCREF(&qb_interface_type);
    Py_INCREF(&qb_object_type);
    PyModule_AddObject(m, "qb_object_t",
                       (PyObject *)&qb_object_type);
    PyModule_AddObject(m, "qb_interface_t",
                       (PyObject *)&qb_interface_type);

    x = PyCapsule_New(&CAPI, qb_object_CAPSULE_NAME, NULL);
    if (x == NULL)
        return NULL;
    PyModule_AddObject(m, "CAPI", x);

    return m;
}
