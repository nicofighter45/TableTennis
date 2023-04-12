#pragma once

#include <vector>
#include <iostream>
#include "C:/Program Files/msys64/mingw64/include/python3.9/Python.h"
#include "vector.h"
#include "constants.h"

void print2DGraph(vector<Vect3D> positions)
{

    Py_Initialize();

    PyObject *pModule = PyImport_ImportModule("2dgraph");
    if (!pModule)
    {
        PyErr_Print();
        exit(1);
    }
    PyObject *pClass = PyObject_GetAttrString(pModule, "2dgraph.py");
    if (!pClass)
    {
        PyErr_Print();
        exit(1);
    }
    PyObject *pInit = PyObject_GetAttrString(pClass, "graph");
    if (!pInit || !PyCallable_Check(pInit))
    {
        PyErr_Print();
        exit(1);
    }

    PyObject *pList = PyList_New(4);
    for (int time = 0; time < positions.size(); time++)
    {
        PyObject *pTuple = PyTuple_New(4);
        PyTuple_SetItem(pTuple, 0, PyFloat_FromDouble(time / spacing_per_second));
        PyTuple_SetItem(pTuple, 1, PyFloat_FromDouble(positions[time].getX()));
        PyTuple_SetItem(pTuple, 2, PyFloat_FromDouble(positions[time].getY()));
        PyTuple_SetItem(pTuple, 3, PyFloat_FromDouble(positions[time].getZ()));
        PyList_Append(pList, pTuple);
    }

    PyObject *pArgs = PyTuple_Pack(positions.size(), pList);
    PyObject *pInstance = PyObject_CallObject(pClass, pArgs);
    if (!pInstance)
    {
        PyErr_Print();
        exit(1);
    }
    Py_XDECREF(pInstance);
    Py_XDECREF(pArgs);
    Py_XDECREF(pList);
    Py_XDECREF(pClass);
    Py_DECREF(pModule);
    Py_Finalize();
}
