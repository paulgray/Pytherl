#include "pytherl.h"

// TODO: implement reload and upgrade functions

static ERL_NIF_TERM hello(ErlNifEnv* env) {
  return enif_make_string(env, hello_from_python());
}

static ERL_NIF_TERM re_match(ErlNifEnv* env, 
                             ERL_NIF_TERM e_subject,
                             ERL_NIF_TERM e_pattern) {
  
  char subject[1024], pattern[1024];
  erl_list_to_string(env, e_subject, subject);
  erl_list_to_string(env, e_pattern, pattern);

  return enif_make_atom(env, re_match_py(subject, pattern) ? "true" : "false");
}

static int load(ErlNifEnv *env, void** priv, ERL_NIF_TERM load_info) {
  Py_Initialize();
  return 0;
}

static void unload(ErlNifEnv* env, void* priv) {
  Py_Finalize();
}

static ErlNifFunc nif_funcs[] = {
  {"hello", 0, hello},
  {"match", 2, re_match}
}; 

ERL_NIF_INIT(pytherl, nif_funcs, load, NULL, NULL, unload)
