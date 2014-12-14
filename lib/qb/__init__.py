import sys
from types import MethodType
from qb._core import qb_object_t, qb_interface_t
from qb._map import qb_map_interface_t
from qb import _memoryregion
class my_object(qb_object_t):
#    class _ifaces(qb_object_t):
#        pass

#    iface = _ifaces

    #@iface_func('(O|O)')
    def do_something(self, *objs):
        pass

    @classmethod
    def register_iface(cls, name, fn):
        print('DOREG {} {} {}'.format(cls, name, fn))
        exec("cls.{} = fn".format(name))

class my_iface(qb_interface_t):
    #@iface_func('(O|O)')
    def do_something(self, *objs):
        pass

def testfn(self, value):
    print('TESTFN {} {} {}'.format(self, value, self.obj))

def main():
#    from qb._memoryregion import MemoryRegion
    my_object.iface.test = my_iface
    qb_object_t.iface.fff = my_iface
#    obj = my_object()
#    obj2 = qb_object_t()
    print(my_object.iface)
#    print(my_iface, obj.ifc.test)
#    print(obj.iface.test)
#    print(obj2.iface.test)
    sys.exit(0)


    m = _memoryregion.MemoryRegion()
    m2 = _memoryregion.MemoryRegion()
    obj = qb_object_t()
    try:
        print(obj.o.fff)
    except AttributeError as e:
        print(e)
    print('get')
#    print(qb_object_t.iface.register) #.fff2 = testfn
    print('end')

    print(obj.iface)
    print(qb_object_t.iface)
    print('OK')
    #print(type(my_iface.iface))
    print('DIC', obj.iface) #fff = my_iface
#    print(qb_object_t.iface)
    #print(obj.iface.fff)


#    obj.iface.test = my_iface
#    obj.iface.test.doreg('fff', testfn)
#    obj.iface.test.fff = testfn
#    obj.iface.test.fff('ha-ha')
    # or
    o2 = my_object()
    print(o2.iface, obj.iface) #.test)
    obj.iface.foo = qb_map_interface_t
    print(issubclass(my_iface, qb_interface_t)) #.o.aaa = 555
    print(obj.iface.foo, obj.iface.foo)
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
