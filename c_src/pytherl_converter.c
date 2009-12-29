#import "pytherl.h"

// TODO - implement tuple and list convertions

ERL_NIF_TERM py_to_erl(ErlNifEnv *env, PyObject *pyObj) {
  if(PyInt_Check(pyObj)) {
    return enif_make_int(env, (int)PyInt_AS_LONG(pyObj));
  } else if(PyBool_Check(pyObj)) {
    return enif_make_atom(env, Py_True == pyObj ? "true" : "false");
  } else if(PyString_Check(pyObj)) {
    return enif_make_string(env, PyString_AsString(pyObj));
  };

  return enif_make_badarg(env);
};
