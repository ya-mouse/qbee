#!/usr/bin/python3.4dm
import sys
import platform

qb_system=platform.system().lower()
qb_machine=platform.machine()
qb_pyversion=platform.python_version().rsplit('.',1)[0]
sys.path.append('build/lib.{}-{}-{}-pydebug'.format(qb_system, qb_machine, qb_pyversion))

import qb

qb.main()
