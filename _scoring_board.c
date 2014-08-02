#include <Python.h>
#include "scoring_board.h"

static char module_docstring[] =
    "This module provides an interface for scoring a scoring a 2048 board by looking ahead at future possible boards.";
static char scoring_board_docstring[] =
    "Recursively look ahead upto a given depth of future boards and calculate the boards score by doing so.";

static PyObject *scoring_board_score(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"score_board", scoring_board_score, METH_VARARGS, scoring_board_docstring},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC 
init_scoring_board(void)
{
    PyObject *m = Py_InitModule3("_scoring_board", module_methods, module_docstring);
    if (m == NULL)
        return;
}

static PyObject *
scoring_board_score(PyObject *self, PyObject *args)
{

  /* Build the output tuple */
  //int board[2][2] = {{10, 1}, {0, 2}};
  
  PyObject *ret = Py_BuildValue("d", score_board());
  return ret;
}
