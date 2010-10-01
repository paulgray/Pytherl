Pytherl
=======
Pytherl is a simple Erlang/C library which allows Erlang developers to re-use Python code inside of their applications.

Compiling
---------
In order to compile the project you should correct the Makefile and insert the proper paths to ERL_INCLUDE and PYTHON_INCLUDE definitions (configure script is on the way). Then type:

	make

Using the library
-----------------
In order to use the library, you should load NIF at first:

	pytherl:init().

Then you can use `pytherl:call(ImportMods, Command, Args)` where

	ImportMods :: string() | list(string()
	Command :: string()
	Args :: list()

For example:

	pytherl:call("re", "re.sub", ["Erlang", "Python", "Hello from Erlang!"]).

is equivalent in Python to

	import re

	re.sub("Erlang", "Python", "Hello from Erlang!")

Moreover, it is possible to convert Python object to the Erlang proplist. Suppose you have the following code written in Python (package pytherl, file pytherl):

	class Foo():
	    bar = 1
	    string = "string"
	    some_list = [1, "c"]

	def get_foo():
	    return Foo()


The following command invoked from the Erlang will result in:

	2> pytherl:call(["pytherl", "pytherl"], "pytherl.pytherl.get_foo", []).
	[{some_list,[1,"c"]},{bar,1},{string,"string"}]

It is also possible to eval the custom Python code (`pytherl:eval(Code, ResultVarName)`) where:

	Code :: string()
	ResultVarName :: string()

`Code` is a valid Python block of code and `ResultVarName` is a name of the variable in the global interpreter dictionary that should be returned as a result of the whole code evaluation.

For example running:

	3> pytherl:eval("a = 1+1", "a").
	2

Will evaluate `a = 1+1` and return a value of variable `a`.
