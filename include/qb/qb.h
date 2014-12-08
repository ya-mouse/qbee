/*
 * QB header
 *
 * Copyright (c) 2014 Anton D. Kachalov <mouse@yandex.ru>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _QB__H
#define _QB__H

#ifdef WIN32
#include <windows.h>
#include <qb/port/dlfcn-win32.h>
#include <qb/port/pthread-win32.h>
#else
#include <pthread.h>
#endif
#include <stdbool.h>
#include <Python.h>

typedef struct {
    PyTypeObject *object_type;
    PyTypeObject *interface_type;
} qb_object_CAPI;

static qb_object_CAPI *qb_object_API = NULL;
#define qb_object_CAPSULE_NAME "qb._core.CAPI"
#define qb_object_IMPORT \
    qb_object_API = (qb_object_CAPI *)PyCapsule_Import(qb_object_CAPSULE_NAME, 0)

#endif
/* EOF _QB__H */
