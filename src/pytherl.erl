-module(pytherl).

-export([init/0]).
-export([hello/0, match/2]).

init() ->
    erlang:load_nif("./priv/pytherl", 0).

hello() ->
    "NOT LOADED".

match(_, _) ->
    not_loaded.
