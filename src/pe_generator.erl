-module(pe_generator).

% TODO - check atom -> string mappings

-include("pytherl.hrl").

-export([generate/1]).

-spec(generate/1 :: (string()) -> any()).
generate(Filename) ->
    Header = header(),

    Form = get_asf(Filename),
    
    Module = get_module_name(Form),
    Exports = get_exports(Form, []),
    FunDefs = get_fun_defs(Form, Exports, []).
    

-spec(get_asf/1 :: (string()) -> list()).
get_asf(Filename) ->
    {ok, Form} = epp:parse_file(Filename, code:lib_dir(pytherl, include), []),
    Form.

-spec(header/0 :: () -> string()).
header() ->
    "#include \"pytherl.hrl\\n\n".

-spec(footer/1 :: (atom()) -> list()).
footer(Name) ->
    ["ERL_NIF_INIT(", atom_to_list(Name), 
     ", nif_funcs, load, NULL, NULL, unload)"].

-spec(get_module_name/1 :: (list()) -> atom()).
get_module_name([{attribute, _, module, Module}|_]) ->
    Module;
get_module_name([_|Rest]) ->
    get_module_name(Rest).

-spec(get_exports/2 :: (list(), list()) -> list()).
get_exports([{attribute, _, export, List}|Rest], Exports) ->
    get_exports(Rest, lists:append(List, Exports));
get_exports([_|Rest], Exports) ->
    get_exports(Rest, Exports);
get_exports([], Exports) ->
    lists:delete({init, 0}, Exports).

-spec(get_fun_defs/3 :: (list(), list(), list()) -> list()).
get_fun_defs([{function, _, Name, Arity, [Clause]}|Rest], Exports, Defs) ->
    case lists:member({Name, Arity}, Exports) of
        true ->
            get_fun_defs(Rest, Exports, 
                         [get_fun_def(Name, Arity, Clause) | Defs]);
        false ->
            get_fun_defs(Rest, Exports, Defs)
    end;
get_fun_defs([_|Rest], Exports, Defs) ->
    get_fun_defs(Rest, Exports, Defs);
get_fun_defs([], _, Defs) ->
    Defs.

-spec(get_fun_def/3 :: (atom(), integer(), tuple()) -> list()).
get_fun_def(Name, Arity, {clause, _, Args, _, Return}) ->    
    Arguments = lists:map(fun get_fun_arg/1, Args),
    Vars = lists:map(fun get_fun_vars/1, Args),

    ["static ERL_NIF_TERM ", atom_to_list(Name), 
     "(ErlNifEnv* env", Arguments, ") {\n"].
    
-spec(get_fun_arg/1 :: ({record, integer(), python_mapping, list()}) -> list()).
get_fun_arg({record, _, python_mapping, Fields}) ->
    {record_field, _, _, {atom, _, Name}} = lists:nth(#python_mapping.name, 
                                                      Fields),
    [", ERL_NIF_TERM ", atom_to_list(Name)].

-spec(get_fun_vars/1 :: ({record, integer(), python_mapping, list()}) -> 
             list()).
get_fun_vars({record, _, python_mapping, Fields}) -> 
    {record_field, _, _, {atom, _, Name}} = 
        lists:nth(#python_mapping.name, Fields),
    {record_field, _, _, {atom, _, ErlangType}} = 
        lists:nth(#python_mapping.erlang_type, Fields),
    {record_field, _, _, {atom, _, PythonType}} = 
        lists:nth(#python_mapping.python_type, Fields),

    get_var_mapping(ErlangType, PythonType, Name).

-spec(get_var_mapping/3 :: (atom(), atom(), atom()) -> list()).
get_var_mapping(integer, integer, Name) ->
    SName = atom_to_list(Name),
    ["int __pytherl_", SName, ";\n",
     "enif_get_int(env, ", SName, ", &__pytherl_", SName, ";\n"];
get_var_mapping(list, string, Name) ->
    SName = atom_to_list(Name),
    ["char __pytherl_", SName, "[10240];\n",
     "erl_list_to_string(env, __pytherl_", SName, ", ", SName, ");\n"];
get_var_mapping(list, list, Name) ->
    SName = atom_to_list(Name),
    ["PyListObject __pytherl_", SName, ";\n",
     "erl_list_to_py_list(env, ", SName, ", &__pytherl_", SName, ");\n"].
%% get_var_mapping(tuple, tuple, Name) ->
%%     SName = atom_to_list(Name),
%%     ["PyTupleObject __pytherl_", SName, ";\n",
%%      "erl_tuple_to_py_tuple(env, ", SName, ", &__pytherl_", SName, ");\n"].
