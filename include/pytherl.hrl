-type(pytherl_type() :: 
      integer | 
      float | 
      atom | 
      bool | 
      string | 
      list | 
      tuple).

-record(python_mapping, {
          name = no_name :: atom() | string(),
          erlang_type :: pytherl_type(),
          python_type :: pytherl_type()
         }).

-record(python_call, {
          module :: undefined | atom(),
          function :: atom(),
          result :: #python_mapping{}
         }).
