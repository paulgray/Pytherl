#import "pytherl.h"

ERL_NIF_TERM py_to_erl(ErlNifEnv *env, PyObject *pyObj) {
  return enif_make_badarg(env);
};
