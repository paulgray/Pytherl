-module(nif_test).

-export([init/0, hello/0]).

init() ->
    erlang:load_nif("./nif_test", 0).

hello() ->
    "NIF library loading failed".
