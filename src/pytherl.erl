-module(pytherl).

-export([init/0]).
-export([call/3]).

-include("pytherl.hrl").

-spec(init/0 :: () -> ok | {error, atom(), string()}).
init() ->
    erlang:load_nif("./priv/pytherl", 0).

-spec(call/3 :: (string() | list(string()), string(), list()) -> term()).
call(Mod, Fun, Args) when is_list(Mod), 
                          is_integer(hd(Mod)) ->
    call([Mod], Fun, Args);
call(Mod, Fun, Args0) ->
    Args1 = string:join(lists:map(fun transform_params/1, Args0), ", "),
    Args = string:right(integer_to_list(length(Args1)), 8, $0) ++
        Args1,
    
    nif_call(string:join(Mod, [$.]), Fun, Args).

-spec(transform_params/1 :: (term()) -> term() | no_return()).
transform_params(Args) when is_list(Args) ->
    case is_string(Args) of
        true ->
            Args;
        false ->
            "[" ++ string:join([transform_params(Arg) || 
                                   Arg <- Args], [$,]) ++ "]"
    end;
transform_params(Args) when is_tuple(Args) ->
    "(" ++ string:join([transform_params(Arg) || 
                           Arg <- tuple_to_list(Args)], [$,]) ++ ",)";
transform_params(Args) when is_binary(Args) ->
    binary_to_list(Args);
transform_params(Args) when is_atom(Args) ->
    atom_to_list(Args);
transform_params(Args) when is_integer(Args) ->
    integer_to_list(Args);
transform_params(Args) when is_float(Args) ->
    float_to_list(Args).

-spec(is_string/1 :: (list()) -> boolean()).
is_string(Term) when is_list(Term) ->
    lists:all(fun(E) when is_integer(E), E < 255, E > -1 ->
                      true;
                 (_) ->
                      false
              end, Term).

-spec(nif_call/3 :: (string(), string(), list()) -> any()).
nif_call(_Mod, _Fun, _Args) ->
    exit(nif_not_loaded).
