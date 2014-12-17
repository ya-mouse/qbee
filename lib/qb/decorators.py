from qb._core import qb_interface_t

def interface(name=None):
    def wrapper(klass):
        if not isinstance(klass, type) or not issubclass(klass, qb_interface_t):
            raise TypeError('value `{}\' have to be subclass of `qb_interface_t\''.format(klass))
        klass.__iface__ = name if isinstance(name, str) else klass.__name__
        return klass

    if isinstance(name, str) or name is None:
        return wrapper

    return wrapper(name)
