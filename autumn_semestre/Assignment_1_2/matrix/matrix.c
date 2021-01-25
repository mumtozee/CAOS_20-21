#define PY_SSIZE_T_CLEAN
#include "Python.h"

typedef Py_ssize_t pszt;

PyObject *
ZeroRow (pszt size)
{
  PyObject *answer = PyList_New (size);
  for (pszt i = 0; i < size; ++i)
    PyList_SetItem (answer, i, PyLong_FromLongLong (0));
  return answer;
}

void
FillWithZeros (pszt passed_size, PyObject *matrix)
{
  const pszt row_size = PyList_Size (matrix);
  for (pszt i = 0; i < row_size; ++i)
    {
      const pszt col_size = PyList_Size (PyList_GetItem (matrix, i));
      if (col_size < passed_size)
        {
          for (pszt j = 0; j < passed_size - col_size; ++j)
            PyList_Append (PyList_GetItem (matrix, i),
                           PyLong_FromLongLong (0));
        }
    }
  for (pszt i = row_size; i < passed_size; ++i)
    PyList_Append (matrix, ZeroRow (passed_size));
}

static PyObject *
dot (PyObject *self, PyObject *args)
{
  if (PyTuple_Size (args) != 3)
    {
      PyErr_SetString (PyExc_TypeError, "Argument count exceeded");
      return NULL;
    }
  pszt input_size;
  PyObject *matrix_a;
  PyObject *matrix_b;

  if (!PyArg_ParseTuple (args, "lOO", &input_size, &matrix_a, &matrix_b))
    return NULL;

  FillWithZeros (input_size, matrix_a);
  FillWithZeros (input_size, matrix_b);
  PyObject *result_matrix = PyList_New (input_size);

  for (pszt i = 0; i < input_size; ++i)
    {
      PyObject *new_row = PyList_New (input_size);
      PyObject *matrix_a_i = PyList_GetItem (matrix_a, i);
      for (pszt j = 0; j < input_size; ++j)
        {
          long long result = 0;
          for (pszt k = 0; k < input_size; ++k)
            {
              PyObject *matrix_b_k = PyList_GetItem (matrix_b, k);
              long long matrix_a_i_k
                  = PyLong_AsLongLong (PyList_GetItem (matrix_a_i, k));
              long long matrix_b_k_j
                  = PyLong_AsLongLong (PyList_GetItem (matrix_b_k, j));
              result += matrix_a_i_k * matrix_b_k_j;
            }
          PyList_SetItem (new_row, j, PyLong_FromLongLong (result));
        }
      PyList_SetItem (result_matrix, i, new_row);
    }

  return result_matrix;
}

static PyMethodDef methods[]
    = { { .ml_name = "dot",
          .ml_meth = dot,
          .ml_flags = METH_VARARGS,
          .ml_doc = "Square Matrix Multiplication Algorithm" },
        { NULL, NULL, 0, NULL } };

static PyModuleDef moduleDef = { .m_base = PyModuleDef_HEAD_INIT,
                                 .m_name = "matrix",
                                 .m_size = -1,
                                 .m_methods = methods };

PyMODINIT_FUNC
PyInit_matrix ()
{
  return PyModule_Create (&moduleDef);
}