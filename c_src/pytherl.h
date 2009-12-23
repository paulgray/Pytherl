#import "Python.h"
#import "erl_nif.h"

// pytherl_utils
void erl_list_to_string(ErlNifEnv *env, ERL_NIF_TERM list, char *string);
void erl_list_to_py_list(ErlNifEnv *env, ERL_NIF_TERM list, PyObject *pyList);
PyObject *get_none();

// hello_python
char *hello_from_python();

// re_run_python
int re_match_py(char *, char *);
