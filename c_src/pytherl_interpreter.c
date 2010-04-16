#import "pytherl.h"

PyObject *pytherl_call(char *mod, char *fun, char *args, int arg_size, ErlNifMutex *mutex) {
  char *command = (char *)calloc(2*(arg_size + strlen(fun)), sizeof(char));
  assert(command);

  enif_mutex_lock(mutex);
  if(!Py_IsInitialized()) {
    Py_Initialize();
  }

  sprintf(command, "import %s", mod);
  PyRun_SimpleString(command);

  sprintf(command, "__pytherl_result = %s(%s)", fun, args);
  PyRun_SimpleString(command);
  enif_mutex_unlock(mutex);

  free(command);

  return pytherl_result(mutex);
};

PyObject *pytherl_eval(char *code, char *var_name, ErlNifMutex *mutex) {
    enif_mutex_lock(mutex);
    if(!Py_IsInitialized()) {
        Py_Initialize();
    }

    PyRun_SimpleString(code);
    enif_mutex_unlock(mutex);

    return pytherl_value(var_name, mutex);
};

inline PyObject *pytherl_result(ErlNifMutex *mutex) {
    return pytherl_value("__pytherl_result", mutex);
};

PyObject *pytherl_value(const char *var_name, ErlNifMutex *mutex) {
    enif_mutex_lock(mutex);
    PyObject *module = PyImport_AddModule("__main__");
    assert(module);
    PyObject *dictionary = PyModule_GetDict(module);
    assert(dictionary);
    PyObject *result = PyDict_GetItemString(dictionary, var_name);
    assert(result);
    enif_mutex_unlock(mutex);

    return result;
};
