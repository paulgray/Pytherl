#import "pytherl.h"

// TODO - implement tuple convertions

ERL_NIF_TERM pytherl_make_list(ErlNifEnv *env, PyObject *obj) {
  Py_ssize_t size = PyList_Size(obj), i;
  ERL_NIF_TERM list = enif_make_list(env, 0);
  
  i = size-1;
  while(i >= 0) {
    ERL_NIF_TERM head = py_to_erl(env, PyList_GetItem(obj, i));
    list = enif_make_list_cell(env, head, list);

    i--;
  };

  return list;
}

ERL_NIF_TERM py_to_erl(ErlNifEnv *env, PyObject *pyObj) {
  if(PyInt_Check(pyObj)) {
    return enif_make_int(env, (int)PyInt_AS_LONG(pyObj));
  } else if(PyBool_Check(pyObj)) {
    return enif_make_atom(env, Py_True == pyObj ? "true" : "false");
  } else if(PyString_Check(pyObj)) {
    return enif_make_string(env, PyString_AsString(pyObj));
  } else if(PyList_Check(pyObj)) {
    return pytherl_make_list(env, pyObj);
  };

  return enif_make_badarg(env);
};
