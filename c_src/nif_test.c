#include "erl_nif.h"
#include "Python.h"

static ERL_NIF_TERM hello(ErlNifEnv* env) {
  return enif_make_string(env, "Hello world!");
}
static ErlNifFunc nif_funcs[] = {
  {"hello", 0, hello}
};

ERL_NIF_INIT(nif_test, nif_funcs, NULL, NULL, NULL, NULL)
