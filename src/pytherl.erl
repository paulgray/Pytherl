-module(pytherl).

-export([init/0]).
-export([call/3, call/2]).
-export([eval/2]).

-on_load(init/0).

%% TODO - implement helper functions for calling native Python funs:
%% call/2

%% TODO - use code:priv_dir(pytherl) to access the driver

-include("pytherl.hrl").

-spec(init/0 :: () -> ok | {error, atom(), string()}).
init() ->
    erlang:load_nif("./priv/libpytherl", 0).

%%
%% Executes the following Python code:
%%   import Imports
%%   Command(Args)
%%
%% where Imports is either a single string or a list of string which will
%% be transformed into a single dotted one.
%%
%% The result of the command execution will be cast to the corresponding
%% Erlang type. Objects will be transformed into a proplists where keys
%% will be object attributes' names and values will be a cast attributes
%% values.
%%
-spec(call/3 :: (string() | list(string()), string(), list()) -> term()).
call(Imports, Command, Args) when is_list(Imports), 
                                  is_integer(hd(Imports)) ->
    call([Imports], Command, Args);
call(Imports, Command, Args0) ->
    Args1 = string:join(lists:map(fun transform_params/1, Args0), ", "),
    Args = string:right(integer_to_list(length(Args1)), 8, $0) ++
        Args1,
    
    nif_call(string:join(Imports, [$.]), Command, Args).

-spec(call/2 :: (string(), list()) -> term()).
call(Command, Args0) ->
    Args1 = string:join(lists:map(fun transform_params/1, Args0), ", "),
    Args = string:right(integer_to_list(length(Args1)), 8, $0) ++
        Args1,

    nif_call(Command, Args).

-spec(eval/2 :: (string(), string()) -> term()).
eval(Command, VarName) ->
    nif_eval(Command, VarName).

-spec(transform_params/1 :: (term()) -> term() | no_return()).
transform_params(Args) when is_list(Args) ->
    case is_string(Args) of
        true ->
            [34 | Args] ++ "\"";
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

-spec(nif_call/2 :: (string(), list()) -> any()).
nif_call(_Fun, _Args) ->
    exit(nif_not_loaded).

-spec(nif_eval/2 :: (string(), string()) -> any()).
nif_eval(_Code, _ResultVarName) ->
    exit(nif_not_loaded).
