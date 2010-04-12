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

PyObject *pytherl_eval(char *code, char *var_name) {
    if(!Py_IsInitialized()) {
        Py_Initialize();
    }

    PyRun_SimpleString(code);

    return pytherl_value(var_name);
};

inline PyObject *pytherl_result() {
    return pytherl_value("__pytherl_result");
};

PyObject *pytherl_value(const char *var_name) {
  PyObject *module = PyImport_AddModule("__main__");
  assert(module);
  PyObject *dictionary = PyModule_GetDict(module);
  assert(dictionary);
  PyObject *result = PyDict_GetItemString(dictionary, var_name);
  assert(result);

  return result;
};
