#include "Python.h"
#include "scoring_board.h"

int
score_board(void)
{
  printf("hello world!\n");
  return 1;
}


/* /\\* */
/*  * Calculates best move and returns it */
/*  *\/ */
/* static PyObject*  */
/* py_rscore(PyObject* self, PyObject* args) */
/* { */

/*   char * tok;         /\* delimiter tokens for strtok *\/ */
/*   int cols;           /\* number of cols to parse, from the left *\/ */
  
/*   int numLines;       /\* how many lines we passed for parsing *\/ */
/*   char * line;        /\* pointer to the line as a string *\/ */
/*   //char * token;       /\* token parsed by strtok *\/ */
  
/*   PyObject * listObj; /\* the list of strings *\/ */
/*   PyObject * strObj;  /\* one string in the list *\/ */
    
/*   /\* the O! parses for a Python object (listObj) checked */
/*      to be of type PyList_Type *\/ */
/*   if (! PyArg_ParseTuple( args, "O!is", &PyList_Type, &listObj,  */
/* 			  &cols, &tok )) return NULL; */
  
/*   /\* get the number of lines passed to us *\/ */
/*   numLines = PyList_Size(listObj); */
  
/*   /\* should raise an error here. *\/ */
/*   if (numLines < 0)	 */
/*     return NULL; /\* Not a list *\/ */
  

/*   /\* iterate over items of the list, grabbing strings, and parsing */
/*      for numbers *\/ */
/*   int i; */
/*   for (i=0; i<numLines; i++){ */

/*     /\* grab the string object from the next element of the list *\/ */
/*     strObj = PyList_GetItem(listObj, i); /\* Can't fail *\/ */
      
/*     /\* make it a string *\/ */
/*     line = PyString_AsString( strObj ); */
      
/*     /\* now do the parsing *\/ */
/*     printf("%s", line); */
/*   } */
    
/*   return Py_BuildValue("d", 47); */
/* } */
