#import "Python.h"

char *hello_from_python() {
  PyRun_SimpleString("from pytherl import pytherl");
  PyRun_SimpleString("result_string = pytherl.hello()");

  PyObject *module = PyImport_AddModule("__main__");
  assert(module);
  
  PyObject *dictionary = PyModule_GetDict(module);
  assert(dictionary);

  PyObject *result = PyDict_GetItemString(dictionary, "result_string");
  assert(result);
  assert(PyString_Check(result));

  return PyString_AsString(result);
};
