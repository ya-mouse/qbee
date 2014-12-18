import sys
from qb._core import qb_object_t, qb_interface_t
from qb.decorators import interface
from qb._map import map_interface
#from qb import _memoryregion

class my_object(qb_object_t):
    @interface('test3')
    class my_iface(qb_interface_t):
        #@iface_func('(O|O)')
        def do_something(self, *objs):
            print('DO SOMETHING', self, objs, self._obj._parent)
            return(self._obj._parent)

class my_ooo(qb_object_t):
    @interface('test')
    class my_iface(my_object.my_iface):
        pass

    @interface
    class map_interface(map_interface):
        pass

class my_obj2(my_object):
    @interface('ttt')
    class my_iface(map_interface):
        pass

def main():
#    from qb._memoryregion import MemoryRegion
#    qb_object_t.iface.fff = my_object.my_iface
    obj = my_ooo('test')
    obj.o.obj2 = my_obj2()
    obj.o.obj3 = my_obj2()
    print(obj.o.obj2.map)
#    print(obj.map, obj.iface.map_interface.add_map(1,2,3,4))
#    print(my_ooo.iface, qb_object_t.iface, my_object.iface)
#    print(dir(my_ooo.iface), dir(my_object.iface), dir(qb_object_t.iface))
#    print(my_object.my_iface, obj.iface.test.do_something())

    print(obj.o.obj2.iface.ttt.add_map(None, None, None, 1))
    print(obj.o.obj3.iface.ttt.add_map(None, None, None, 2))
    print(obj.iface.test.do_something())
##    print(my_ooo.iface.test, obj.iface.test, 'OK')
#    # qb_preobject_t('template%')
#    print(dir(obj2.iface))
#    obj2.o.test = 123
#    print(issubclass(my_object.my_iface, qb_interface_t))
    print('EXIT')
    sys.exit(0)
