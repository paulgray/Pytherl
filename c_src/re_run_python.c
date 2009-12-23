#import "Python.h"

PyObject *get_none() {
  Py_RETURN_NONE;
}

int re_match_py(char *subject, char *pattern) {
  char command[4096];

  sprintf(command, "result = re.match(\"%s\", \"%s\")", pattern, subject);

  PyRun_SimpleString("import re");
  PyRun_SimpleString(command);

  PyObject *module = PyImport_AddModule("__main__");
  assert(module);
  
  PyObject *dictionary = PyModule_GetDict(module);
  assert(dictionary);

  PyObject *result = PyDict_GetItemString(dictionary, "result");
  assert(result);

  return get_none() != result;
};
