#include "Python.h"
#include "erl_nif.h"

// TODO: implement reload and upgrade functions

char *hello_from_python();

static ERL_NIF_TERM hello(ErlNifEnv* env) {
  return enif_make_string(env, hello_from_python());
}

static int load(ErlNifEnv *env, void** priv, ERL_NIF_TERM load_info) {
  Py_Initialize();
  return 0;
}

static void unload(ErlNifEnv* env, void* priv) {
  Py_Finalize();
}

static ErlNifFunc nif_funcs[] = {
  {"hello", 0, hello}
}; 

ERL_NIF_INIT(pytherl, nif_funcs, load, NULL, NULL, unload)
