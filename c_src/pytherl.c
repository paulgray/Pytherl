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

  if(args == NULL) {
    return enif_make_badarg(env);
  };

  //  pytherl_call(mod, fun, args);

  ERL_NIF_TERM ret = enif_make_atom(env, args);
  free(args);

  return ret;
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
