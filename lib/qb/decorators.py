#
# Generic decorators
#
# Copyright (c) 2014 Anton D. Kachalov <mouse@yandex.ru>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
from qb._core import qb_interface_t, qb_pin_t

def interface(name=None):
    def wrapper(klass):
        if not isinstance(klass, type) or not issubclass(klass, qb_interface_t):
            raise TypeError('value `{}\' have to be subclass of `qb_interface_t\''.format(klass))
        klass.__iface__ = name if isinstance(name, str) else klass.__name__
        return klass

    if isinstance(name, str) or name is None:
        return wrapper

    return wrapper(name)

def pin(name=None):
    def wrapper(klass):
        if not isinstance(klass, type) or not issubclass(klass, qb_pin_t):
            raise TypeError('value `{}\' have to be subclass of `qb_pin_t\''.format(klass))
        klass.__pin__ = name if isinstance(name, str) else klass.__name__
        return klass

    if isinstance(name, str) or name is None:
        return wrapper

    return wrapper(name)
