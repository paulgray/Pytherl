#include "pytherl.h"

// TODO: implement reload and upgrade functions

static ERL_NIF_TERM nif_call(ErlNifEnv* env, 
                             ERL_NIF_TERM e_mod,
                             ERL_NIF_TERM e_fun,
                             ERL_NIF_TERM e_args) {
  
  char mod[4096], fun[4096];
  erl_list_to_string(env, e_mod, mod);
  erl_list_to_string(env, e_fun, fun);
  char *args = erl_arg_list_to_string(env, e_args);

  pytherl_call(mod, fun, args);

  free(args);

  return "something";
}

static int load(ErlNifEnv *env, void** priv, ERL_NIF_TERM load_info) {
  Py_Initialize();
  return 0;
}

static void unload(ErlNifEnv* env, void* priv) {
  Py_Finalize();
}

static ErlNifFunc nif_funcs[] = {
  {"nif_call", 3, nif_call}
}; 

ERL_NIF_INIT(pytherl, nif_funcs, load, NULL, NULL, unload)
