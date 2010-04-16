#ifndef _PYTHERL_H_
#define _PYTHERL_H_

#import "Python.h"
#import "erl_nif.h"

#define PREFIX_LEN 8
#define PYTHERL_SEGMENT_SIZE 4096

// pytherl_utils
void erl_list_to_string(ErlNifEnv *env, ERL_NIF_TERM list, char *string);
char *erl_arg_list_to_string(ErlNifEnv *env, ERL_NIF_TERM list, int *arg_size);

// pytherl_interpreter
PyObject *pytherl_call(char *mod, char *fun, char *args, int arg_size, ErlNifMutex *mutex);
PyObject *pytherl_eval(char *code, char *result_var_name, ErlNifMutex *mutex);
PyObject *pytherl_value(const char *var_name, ErlNifMutex *mutex);
inline PyObject *pytherl_result(ErlNifMutex *mutex);

// pytherl_converter
ERL_NIF_TERM py_to_erl(ErlNifEnv* env, PyObject *pyObj);

#endif // _PYTHERL_H_
