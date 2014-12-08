import sys
from qb._core import qb_object_t, qb_interface_t
from qb import _memoryregion

class my_iface(qb_interface_t):
    #@iface_func('(O|O)')
    def do_something(self, *objs):
        pass

def main():
#    from qb._memoryregion import MemoryRegion
    m = _memoryregion.MemoryRegion()
    m2 = _memoryregion.MemoryRegion()
    obj = qb_object_t()
    try:
        print(obj.o.fff)
    except AttributeError as e:
        print(e)
    obj.iface.test = qb_interface_t
    # or
    obj.iface.foo = _memoryregion.MemoryRegion
    print(issubclass(my_iface, qb_interface_t)) #.o.aaa = 555
    print(obj.iface.test, obj.iface.foo)
    obj.iface.ffff = qb_interface_t
    print(obj.iface.ffff)
    #print(dir(m2.o))
#    m2.iface.ffff = 'fff'
#    print(m.iface.ffff, m2.iface.ffff)
#    m.map += [0,1,2]
#    m2 = _memoryregion.MemoryRegion()
#    m.iface.memoryspace = _memoryregion
#    m.o.fff = 23
#    m2.o.fff = 99
#    print(m.map, m.iface.register(), m.iface.register())
#    print(m.iface.f1, m.iface.f2, m.o.fff, m2.o.fff)
#    m.o.fff = 55
#    print(m.register())
#    print(m.o.fff)
