#import "pytherl.h"

void erl_list_to_string(ErlNifEnv *env, ERL_NIF_TERM list, char *string) {
  ERL_NIF_TERM head, tail;
  int character;

  while(enif_get_list_cell(env, list, &head, &tail)) {
    if(!enif_get_int(env, head, &character)) {
      return;
    }

    *string++ = (char)character;
    list = tail;
  };

  *string = '\0';
};

char *erl_arg_list_to_string(ErlNifEnv *env,
                             ERL_NIF_TERM list) {
  ERL_NIF_TERM head, tail;
  char str_length[PREFIX_LEN], *args;
  int i, length, character;
  
  for(i=0; i<PREFIX_LEN; i++) {
    if(enif_get_list_cell(env, list, &head, &tail)) {
      if(!enif_get_int(env, head, &character)) {
        return NULL;
      }

      str_length[i] = (char)character;
      list = tail;
    } else {
      return NULL;
    }
  };

  length = atoi(str_length);
  args = (char *)malloc(sizeof(char)*length);

  erl_list_to_string(env, list, args);

  return args;
};

void erl_list_to_py_list(ErlNifEnv *env, 
                         ERL_NIF_TERM list, 
                         PyObject *pyList) {
  ERL_NIF_TERM head, tail;
  int character, size = 0;
  
  while(enif_get_list_cell(env, list, &head, &tail)) {
    if(!enif_get_int(env, head, &character)) {
      return;
    };

    PyObject *integer = (PyObject *)malloc(sizeof(PyIntObject));
    int ret = PyList_SetItem(pyList, size++, integer);
    assert(ret == 0);

    list = tail;
  };
};

PyObject *get_none() {
  Py_RETURN_NONE;
}
