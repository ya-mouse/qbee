import os
from setuptools import setup, find_packages
from distutils.core import Extension
from distutils.command.build_py import build_py_2to3 as build_py

dependency_links = []
install_requires = []

EXCLUDE_PY = []

with open('requirements.txt') as f:
    for line in f:
        if line.startswith("#"):
            continue
        if '#egg=' in line:
            dependency_links.append(line)
            continue
        install_requires.append(line)

class PCTBuildPy(build_py):
    def find_package_modules(self, package, package_dir, *args, **kwargs):
        modules = build_py.find_package_modules(self, package, package_dir,
            *args, **kwargs)

        # Exclude certain modules
        retval = []
        for item in modules:
            pkg, module = item[:2]
            if (pkg, module) in EXCLUDE_PY:
                continue
            retval.append(item)
        return retval

include_dirs = [ 'include' ]

exts = [
    Extension('_core', sources=['src/_core.c']),
    Extension('_map', sources=['src/_map.c']),
    Extension('_memoryregion', sources=['src/_memoryregion.c']),
]

setup(
    name='python-qb',
    version='0.1',
    description='QB python modules',
    author='Anton D. Kachalov',
    cmdclass={'build_py': PCTBuildPy},
    include_dirs = include_dirs,
    package_dir = {'qb': 'lib/qb'},
    packages=['qb'],
    ext_package='qb',
    ext_modules=exts,
    platforms='any',
    zip_safe=False,
    include_package_data=True,
    dependency_links=dependency_links,
    install_requires=install_requires,
)