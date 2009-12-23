-module(pytherl).

-export([init/0]).
-export([hello/0]).

init() ->
    erlang:load_nif("./priv/pytherl", 0).

hello() ->
    "ERLANG".
