#include <Python.h>
#include <numpy/arrayobject.h>
#include "scoring_board.h"

extern int max_depth;

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

  /* Load numpy functionality */
  import_array();
}

static PyObject *
scoring_board_score(PyObject *self, PyObject *args)
{

  PyObject *board;
  
  /* Parse the input tuple */
  if (!PyArg_ParseTuple(args, "iO", &max_depth, &board))
    return NULL;

  /* Interpret the input objects as numpy arrays. */
  board = PyArray_FROM_OTF(board, NPY_INT, NPY_IN_ARRAY);
    
  /* If that didn't work, throw an exception. */
  if (board == NULL) {
    Py_XDECREF(board);
    return NULL;
  }

  /* How many data points are there? */
  int X = (int)PyArray_DIM(board, 0);
  int Y = (int)PyArray_DIM(board, 1);

  /* Get pointers to the data as C-types. */
  int *c_board  = (int*)PyArray_DATA(board);

  /* Generate 2 dimensional array */
  //int *c_ptrs[X];
  /* int n; */
  /* for (n = 0; n < X; n++){ */
  /*   c_ptrs[n] = c_board + n*Y; */
  /* } */

  /* int* board_ptrs[Y]; */
  /* int i; */
  /* for (i = 0; i < Y; i++){ */
  /*   board_ptrs[i] = c_board + i*X; */
  /* } */

  /* int i, j; */
  /* printf("Received WHA Board:\n"); */
  /* for (i = 0; i < X; i++){ */
  /*   for (j = 0; j < Y; j++){ */
  /*     printf("%d\t", c_ptrs[i][j]); */
  /*   } */
  /*   printf("\n"); */
  /* } */

  /* Call the external C function to compute the board score. */
  float score = score_board(c_board, X, Y, 0);

  /* Clean up. */
  Py_DECREF(board);

  /* Build the output tuple */
  PyObject *py_score = Py_BuildValue("f", score);
  return py_score;
}
