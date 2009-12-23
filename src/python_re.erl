-module(python_re).

-include("pytherl.hrl").

-export([init/0]).
-export([sub/3, escape/1]).

init() ->
    erlang:load_nif("./priv/python_re", 0).

sub(#python_mapping{name = pattern,
                    erlang_type = string,
                    python_type = string}, 
    #python_mapping{name = replacement,
                    erlang_type = string,
                    python_type = string},
    #python_mapping{name = subject,
                    erlang_type = string,
                    python_type = string}) ->
    #python_call{module = re,
                 function = sub,
                 result = #python_mapping{python_type = string,
                                          erlang_type = string}}.

escape(#python_mapping{name = string,
                       erlang_type = string,
                       python_type = string}) ->
    #python_call{module = re,
                 function = escape,
                 result = #python_mapping{python_type = string,
                                          erlang_type = string}}.
