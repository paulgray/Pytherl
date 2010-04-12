#import "pytherl.h"

// TODO: implement reload and upgrade functions

static ERL_NIF_TERM nif_call(ErlNifEnv* env, 
                             int argc,
                             const ERL_NIF_TERM argv[]) {
  
  int arg_size;
  char mod[4096], fun[4096];

  assert(argc == 3);

  erl_list_to_string(env, argv[0], mod);
  erl_list_to_string(env, argv[1], fun);
  char *args = erl_arg_list_to_string(env, argv[2], &arg_size);

  if(args == NULL) {
      return enif_make_badarg(env);
  };

  PyObject *pyRes = pytherl_call(mod, fun, args, arg_size);
  ERL_NIF_TERM erlRes = py_to_erl(env, pyRes);

  free(args);

  return erlRes;
}

static ERL_NIF_TERM nif_call_native(ErlNifEnv *env,
                                    int argc,
                                    const ERL_NIF_TERM argv[]) {
  return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM nif_eval(ErlNifEnv *env,
                             int argc,
                             const ERL_NIF_TERM argv[]) {
    

    return enif_make_atom(env, "ok");
}

static int load(ErlNifEnv *env, void** priv, ERL_NIF_TERM load_info) {
  //  Py_Initialize();
  return 0;
}

static void unload(ErlNifEnv* env, void* priv) {
  //  Py_Finalize();
}

static ErlNifFunc nif_funcs[] = {
  {"nif_call", 3, nif_call},
  {"nif_call", 2, nif_call_native},
  {"nif_eval", 1, nif_eval}
}; 

ERL_NIF_INIT(pytherl, nif_funcs, load, NULL, NULL, unload)
