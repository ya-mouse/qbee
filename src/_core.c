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
    PyObject *iface_defs;
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
    PyObject *cls, *name;
    PyObject *tuple = PyTuple_Pack(1, self);
//    self->ob_dict = PyDict_New();
    printf("self=%p t=%p\n", self, ((PyObject *)(&qb_interface_set_type))->ob_type);
    self->objset = PyObject_CallObject((PyObject *) &qb_object_set_type, NULL); //(PyObject *)self);
    self->ifaceset = PyObject_CallObject((PyObject *) &qb_interface_set_type, tuple); // (PyObject *)self);
    Py_DECREF(tuple);
    cls = Py_TYPE(self);
    printf("OBJINIT: self=%p cls=%p\n", self, cls);
    name = PyUnicode_InternFromString("ifc");
    printf("--%d\n", PyObject_SetAttr((PyObject *)self, name, self->ifaceset));
/*
    if (PyObject_HasAttr(cls, name)) {
        Py_ssize_t i = 0;
        PyObject *key, *value, *obj;
        qb_interface_t *iface;
        printf("cls has\n");
        iface = Py_TYPE(PyObject_GetAttr((PyObject *)cls, name))->tp_dict;
        while (PyDict_Next(iface, &i, &key, &value)) {
            obj = PyObject_CallObject(iface, NULL); // PyTuple_Pack(1, self->parentobj));
            printf("%d\n", obj); PyObject_SetAttr((PyObject *)Py_TYPE(self), key, obj);
        }
    }
*/
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
    Py_ssize_t i;
    PyObject *key, *value, *iface, *obj;

    printf("init=%p\n", args);
    if (args == NULL)
        return 0;

    self->parentobj = PyTuple_GetItem(args, 0);
    fprintf(stderr, "interface_set_init: %p %p t=%p\n", self, self->iface_defs, self);
    if (self->iface_defs == NULL)
        return 0;

    iface = self->iface_defs;
    self->iface_defs = PyDict_New();
    self->iface = iface;
    i = 0;
    printf("ifc=%p\n", self->iface);
    while (PyDict_Next(self->iface, &i, &key, &value)) {
        obj = PyObject_CallObject(iface, NULL); // PyTuple_Pack(1, self->parentobj));
        printf("%d\n", obj); PyObject_GenericSetAttr((PyObject *)Py_TYPE(self), key, obj);
    }
    return 0;
}

static PyObject *
_Py_interface_set_getattr(qb_interface_set_t *self, PyObject *name)
{
    fprintf(stderr, "GETATTR: %p\n", self);
    return Py_None;
}

static int
_Py_interface_set_setattr(qb_interface_set_t *self, PyObject *name, PyObject *iface)
{
    PyObject *obj;

    printf("check=%d\n", Py_TYPE(self) == &qb_interface_set_type);

#if 0
    if (PyType_Check(iface) && !PyType_Check(self) && self->parentobj == NULL) {
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
        if (self->parentobj != NULL) {
            PyErr_SetString(PyExc_TypeError, "it is not possible to change interface definition on instantiated class");
        } else {
            PyErr_SetString(PyExc_TypeError, "value must be class (subclass of `qb_interface_t')");
        }
        return -1;
#endif
    }
#endif
    //obj = PyObject_CallObject(iface, NULL);// PyTuple_Pack(1, self->parentobj));
    if (self->iface_defs == NULL)
        self->iface_defs = PyDict_New();
    fprintf(stderr, "IFACESET: t=%p %p\n", Py_TYPE(self), self);

    return PyObject_GenericSetAttr((PyObject *)self, name, iface);
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
    fprintf(stderr, "OBJSET %p\n", self);
    PyObject_GenericSetAttr((PyObject *)self, name, item);
    return 0;
}

static PyObject *
_Py_object_set_getattr(PyObject *self, PyObject *name)
{
    fprintf(stderr, "GGGG: %p\n", self);
    return PyObject_GenericGetAttr(self, name);
}

static int
_Py_object_set_descrset(PyObject *meth, PyObject *obj, PyObject *cls)
{
    fprintf(stderr, "SET %p\n", obj);
    return 0;
}

static PyObject *
_Py_interface_set_descrget(PyObject *meth, PyObject *obj, PyObject *cls)
{
    fprintf(stderr, "GET self=%p obj=%p cls=%p t=%s %s %p\n", meth, obj, cls, Py_TYPE(meth)->tp_name, ((PyTypeObject*)(cls))->tp_name, &qb_interface_set_type);
    if (obj != NULL) {
        PyObject *iface = ((qb_interface_set_t *)obj)->iface;
        Py_INCREF(iface);
        printf("---%p\n", iface);
        return iface;
    } else {
        PyObject *name = PyUnicode_InternFromString("ifc");
        Py_INCREF(name);
        if (!PyObject_HasAttr(cls, name)) {
            obj = PyObject_CallObject((PyObject *) &qb_interface_set_type, PyTuple_Pack(1, Py_None)); // (PyObject *)self);
            printf("NIFAC: %p\n", obj);
            printf("S--%p %d\n", cls, PyObject_GenericSetAttr(cls, name, obj));
            printf("has=%p\n", _PyType_Lookup(cls, name));
        } else {
            obj = PyObject_GenericGetAttr(cls, name);
        }
        Py_DECREF(name);
        printf("GIFACE: %p\n", obj);
        return obj;
    }
    return meth;
}

static PyObject *
_Py_interface_set_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *p;
    p = PyType_GenericNew(type, args, kwds);
    fprintf(stderr, "IFACE_NEW: t=%p a=%p o=%p\n", type, args, p);
    return p;
}

static PyObject *
_Py_interface_set_register(PyObject *self, PyObject *args)
{
    return self;
}

static PyMemberDef _Py_object_members[] = {
//    {"iface", T_OBJECT, offsetof(qb_object_t, ifaceset), READONLY},
    {"o", T_OBJECT, offsetof(qb_object_t, objset), READONLY},
    {NULL}
};

static PyMemberDef _Py_interface_members[] = {
    {"obj", T_OBJECT, offsetof(qb_interface_t, parentobj), READONLY},
    {NULL}
};

static PyMethodDef _Py_interface_set_methods[] = {
    {"register", (PyCFunction)_Py_interface_set_register, METH_VARARGS,
        PyDoc_STR("register interface")},
    {NULL}
};

PyTypeObject qb_object_type = {
    .ob_base      = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "qb_object_t",
    .tp_basicsize = sizeof(qb_object_t),
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HEAPTYPE,
    .tp_doc       = PyDoc_STR("qb_object_t"),
    .tp_traverse  = (traverseproc)_Py_object_traverse,
    .tp_descr_set = _Py_object_set_descrset,
//    .tp_descr_get = _Py_object_set_descrget,
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
    .tp_members   = _Py_interface_members,
    .tp_descr_set = _Py_object_set_descrset,
//    .tp_descr_get = _Py_object_set_descrget,
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
//    .tp_getattro  = (getattrofunc)_Py_object_set_getattr,
    .tp_dictoffset = offsetof(qb_object_set_t, o),
    .tp_init      = (initproc)_Py_object_set_init,
    .tp_new       = PyType_GenericNew,
};

PyTypeObject qb_interface_set_type = {
    .ob_base      = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "qb_interface_set_t",
    .tp_basicsize = sizeof(qb_interface_set_t),
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE,
    .tp_doc       = PyDoc_STR("qb_inetrace_set_t"),
    .tp_setattro  = (setattrofunc)_Py_interface_set_setattr,
    .tp_getattro  = (getattrofunc)_Py_interface_set_getattr,
    .tp_descr_get = _Py_interface_set_descrget,
    .tp_dictoffset = offsetof(qb_interface_set_t, iface_defs),
    .tp_init      = (initproc)_Py_interface_set_init,
    .tp_new       = _Py_interface_set_new,
//    .tp_base      = &PyBaseObject_Type,
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
    PyObject *m, *d, *x, *descr, *meth;

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

    descr = PyType_GenericAlloc(&qb_interface_set_type, 0);
    printf("TYPE: t=%p desc=%p\n", Py_TYPE(descr), descr);
    Py_INCREF(descr);
    PyDict_SetItemString(qb_object_type.tp_dict, "iface", descr);

/*
    descr = PyDescr_NewMember(&qb_object_type, &_Py_object_members[0]);
    printf("%d\n", PyDict_SetItemString(qb_object_type.tp_dict, "iface", descr));
    meth = PyDescr_NewMethod(Py_TYPE(descr), &_Py_interface_set_methods[0]);
    printf("%d\n", PyDict_SetItemString(Py_TYPE(descr)->tp_dict, "register", meth));
*/
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
