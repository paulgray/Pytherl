#import "pytherl.h"

PyObject *pytherl_call(char *mod, char *fun, char *args, int arg_size) {
  char *command = (char *)malloc(sizeof(char)*(arg_size + strlen(fun) + 16));
  assert(command);

  PyImport_ImportModule(mod);

  sprintf(command, "result = %s(%s)", fun, args);
  PyRun_SimpleString(command);

  free(command);

  PyObject *module = PyImport_AddModule("__main__");
  assert(module);
  PyObject *dictionary = PyModule_GetDict(module);
  assert(dictionary);
  PyObject *result = PyDict_GetItemString(dictionary, "result");
  assert(result);

  return result;
};
