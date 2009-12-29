#ifndef _PYTHERL_H_
#define _PYTHERL_H_

#import "Python.h"
#import "erl_nif.h"

#define PREFIX_LEN 8

// pytherl_utils
void erl_list_to_string(ErlNifEnv *env, ERL_NIF_TERM list, char *string);
char *erl_arg_list_to_string(ErlNifEnv *env, ERL_NIF_TERM list, int *arg_size);
void erl_list_to_py_list(ErlNifEnv *env, ERL_NIF_TERM list, PyObject *pyList);
PyObject *get_none();

// pytherl_interpreter
PyObject *pytherl_call(char *mod, char *fun, char *args, int arg_size);

// pytherl_converter
ERL_NIF_TERM py_to_erl(ErlNifEnv* env, PyObject *pyObj);

// hello_python
char *hello_from_python();

// re_run_python
int re_match_py(char *, char *);

#endif // _PYTHERL_H_
