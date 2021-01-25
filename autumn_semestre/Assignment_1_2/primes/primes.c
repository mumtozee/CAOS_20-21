#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <math.h>
#include <stdint.h>

typedef Py_ssize_t pszt;

static PyObject *
prime_list (int64_t input)
{
  PyObject *result = PyList_New (0);
  PyList_Append (result, PyLong_FromLongLong (1));
  while (input % 2 == 0)
    {
      PyList_Append (result, PyLong_FromLongLong (2));
      input /= 2;
    }
  int64_t upper_bound = sqrt (input);
  for (int64_t i = 3; i <= upper_bound; i += 2)
    {
      while (input % i == 0)
        {
          PyList_Append (result, PyLong_FromLongLong (i));
          input /= i;
        }
    }
  if (input > 2)
    PyList_Append (result, PyLong_FromLongLong (input));

  return result;
}

static PyObject *
factor_out (PyObject *self, PyObject *args)
{
  int64_t num_to_proceed = 0;

  if (!PyArg_ParseTuple (args, "L", &num_to_proceed))
    return NULL;

  if (num_to_proceed < 1)
    {
      PyErr_SetString (PyExc_ValueError, "Invalid value passed");
      return NULL;
    }

  PyObject *result_factor = prime_list (num_to_proceed);

  if (PyList_Size (result_factor) == 2)
    {
      Py_XDECREF (result_factor);
      return PyUnicode_FromString ("Prime!");
    }
  else
    {
      return result_factor;
    }
}

static PyMethodDef methods[]
    = { { .ml_name = "factor_out",
          .ml_meth = factor_out,
          .ml_flags = METH_VARARGS,
          .ml_doc = "Integers factoring out algorithm" },
        { NULL, NULL, 0, NULL } };

static PyModuleDef moduleDef = { .m_base = PyModuleDef_HEAD_INIT,
                                 .m_name = "primes",
                                 .m_size = -1,
                                 .m_methods = methods };

PyMODINIT_FUNC
PyInit_primes ()
{
  return PyModule_Create (&moduleDef);
}