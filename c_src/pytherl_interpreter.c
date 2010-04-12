#import "pytherl.h"

PyObject *pytherl_call(char *mod, char *fun, char *args, int arg_size) {
  char *command = (char *)calloc(2*(arg_size + strlen(fun)), sizeof(char));
  assert(command);

  if(!Py_IsInitialized()) {
    Py_Initialize();
  }

  sprintf(command, "import %s", mod);
  PyRun_SimpleString(command);

  sprintf(command, "__pytherl_result = %s(%s)", fun, args);
  PyRun_SimpleString(command);

  free(command);

  return pytherl_result();
};

PyObject *pytherl_result() {
  PyObject *module = PyImport_AddModule("__main__");
  assert(module);
  PyObject *dictionary = PyModule_GetDict(module);
  assert(dictionary);
  PyObject *result = PyDict_GetItemString(dictionary, "__pytherl_result");
  assert(result);

  return result;
};
