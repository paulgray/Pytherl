#import "erl_nif.h"

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
