#import "Python.h"
#import "erl_nif.h"

// pytherl_utils
void erl_list_to_string(ErlNifEnv *env, ERL_NIF_TERM list, char *string);
char *erl_arg_list_to_string(ErlNifEnv *env, ERL_NIF_TERM list);
void erl_list_to_py_list(ErlNifEnv *env, ERL_NIF_TERM list, PyObject *pyList);
PyObject *get_none();

// pytherl_interpreter
ERL_NIF_TERM pytherl_call(char *mod, char *fun, char *args);

// hello_python
char *hello_from_python();

// re_run_python
int re_match_py(char *, char *);
