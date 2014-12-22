#
# RPC client/server routines
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
import sys
import os
import rpyc
from rpyc.utils.server import ThreadedServer, ForkingServer
from rpyc.utils.classic import DEFAULT_SERVER_PORT, DEFAULT_SERVER_SSL_PORT
from rpyc.utils.classic import connect, ssl_connect
from rpyc.utils.registry import REGISTRY_PORT
from rpyc.utils.registry import UDPRegistryClient
from rpyc.utils.authenticators import SSLAuthenticator
from rpyc.lib import setup_logger
from rpyc.core import SlaveService

class qb_dom0_server:
    def __init__(self, host="", port=None, forking=False, registry_host=None, ipv6=True,
                 ssl_keyfile=None, ssl_certfile=None, ssl_cafile=None):
        self._host = host
        self._port = port
        self._ipv6 = ipv6
        self._registry_host = registry_host
        self._ssl_keyfile = ssl_keyfile
        self._ssl_certfile = ssl_certfile
        self._ssl_cafile = ssl_cafile

        if self._registry_host is None:
            self._registry_host = '255.255.255.255'
        self._registry_port = REGISTRY_PORT
        self._registrar = UDPRegistryClient(ip = self._registry_host, port = self._registry_port)

        if self._ssl_keyfile:
            self._authenticator = SSLAuthenticator(self._ssl_keyfile, self._ssl_certfile,
                self._ssl_cafile)
            default_port = DEFAULT_SERVER_SSL_PORT
        else:
            self._authenticator = None
            default_port = DEFAULT_SERVER_PORT
        if self._port is None:
            self._port = default_port

        setup_logger(False, None)

        factory = ForkingServer if forking else ThreadedServer
        self._t = factory(SlaveService, hostname = self._host, port = self._port,
                          reuse_addr = True, ipv6 = self._ipv6, authenticator = self._authenticator, 
                          registrar = self._registrar, auto_register = False)

    def start(self):
        self._t.start()

class qb_dom0_client:
    def __init__(self, host, port=None, ipv6=True,
                 ssl_keyfile=None, ssl_certfile=None, ssl_cafile=None):
        self._host = host
        self._port = port
        self._ipv6 = ipv6
        self._ssl_keyfile = ssl_keyfile
        self._ssl_certfile = ssl_certfile
        self._ssl_cafile = ssl_cafile

        if self._ssl_keyfile:
            self._authenticator = SSLAuthenticator(self._ssl_keyfile, self._ssl_certfile,
                self._ssl_cafile)
            default_port = DEFAULT_SERVER_SSL_PORT
        else:
            self._authenticator = None
            default_port = DEFAULT_SERVER_PORT
        if self._port is None:
            self._port = default_port

        if self._ssl_keyfile:
            self._conn = ssl_connect(self._host, port = default_port, keyfile = self._ssl_keyfile,
                                    certfile = self._ssl_certfile, ca_certs = self._ssl_ca_certs, ipv6 = self._ipv6)
        else:
            self._conn = connect(self._host, port = default_port, ipv6 = self._ipv6, keepalive = True)

    @property
    def root(self):
        return self._conn.modules.qb.root
