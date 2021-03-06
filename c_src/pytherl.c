#import "pytherl.h"

// TODO: implement reload and upgrade functions

static ERL_NIF_TERM nif_call(ErlNifEnv* env, 
                             int argc,
                             const ERL_NIF_TERM argv[]) {
  
  int arg_size;
  char mod[PYTHERL_SEGMENT_SIZE], fun[PYTHERL_SEGMENT_SIZE];

  assert(argc == 3);

  erl_list_to_string(env, argv[0], mod);
  erl_list_to_string(env, argv[1], fun);
  char *args = erl_arg_list_to_string(env, argv[2], &arg_size);

  if(args == NULL) {
      return enif_make_badarg(env);
  };

  ErlNifMutex *interpreter_mutex = (ErlNifMutex *) enif_priv_data(env);
  PyObject *pyRes = pytherl_call(mod, fun, args, arg_size, interpreter_mutex);
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
    char code[PYTHERL_SEGMENT_SIZE >> 4], result_var_name[PYTHERL_SEGMENT_SIZE];
    
    assert(argc == 2);
    
    erl_list_to_string(env, argv[0], code);
    erl_list_to_string(env, argv[1], result_var_name);

    ErlNifMutex *interpreter_mutex = (ErlNifMutex *) enif_priv_data(env);
    PyObject *pyRes = pytherl_eval(code, result_var_name, interpreter_mutex);
    ERL_NIF_TERM erlRes = py_to_erl(env, pyRes);

    return erlRes; 
}

static int load(ErlNifEnv *env, void** priv, ERL_NIF_TERM load_info) {
    ErlNifMutex *interpreter_mutex = enif_mutex_create("interpreter_mutex");
    assert(interpreter_mutex);

    *priv = interpreter_mutex;
    
    return 0;
}

static void unload(ErlNifEnv* env, void* priv) {
    ErlNifMutex *interpreter_mutex = (ErlNifMutex *) priv;

    enif_mutex_lock(interpreter_mutex);
    Py_Finalize();
    enif_mutex_unlock(interpreter_mutex);
}

static ErlNifFunc nif_funcs[] = {
  {"nif_call", 3, nif_call},
  {"nif_call", 2, nif_call_native},
  {"nif_eval", 2, nif_eval}
}; 

ERL_NIF_INIT(pytherl, nif_funcs, load, NULL, NULL, unload)
