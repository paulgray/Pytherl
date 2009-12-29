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
                             ERL_NIF_TERM list,
                             int *arg_size) {
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

  length = atoi(str_length)+1;
  args = (char *)calloc(length, sizeof(char));

  erl_list_to_string(env, list, args);
  *arg_size = length;

  return args;
};
