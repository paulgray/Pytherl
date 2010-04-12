#import "pytherl.h"

// TODO - implement tuple convertions

ERL_NIF_TERM pytherl_make_tuple(ErlNifEnv *env, PyObject *obj) {
    Py_ssize_t size = PyTuple_Size(obj), i;
    ERL_NIF_TERM *array = (ERL_NIF_TERM *) malloc(sizeof(ERL_NIF_TERM)*size);
    for(i = 0; i<size; i++) {
        array[i] = py_to_erl(env, PyTuple_GetItem(obj, i));
    };

    ERL_NIF_TERM tuple = enif_make_tuple_from_array(env, array, size);
    free(array);

    return tuple;
};

ERL_NIF_TERM pytherl_make_list(ErlNifEnv *env, PyObject *obj) {
  Py_ssize_t size = PyList_Size(obj), i;
  ERL_NIF_TERM list = enif_make_list(env, 0);
  
  i = size-1;
  while(i >= 0) {
    ERL_NIF_TERM head = py_to_erl(env, PyList_GetItem(obj, i));
    list = enif_make_list_cell(env, head, list);

    i--;
  };

  return list;
};

ERL_NIF_TERM pytherl_make_proplist(ErlNifEnv *env, PyObject *obj) {
  ERL_NIF_TERM list = enif_make_list(env, 0);
  PyObject* pyList = PyDict_Items(obj);
  int size = PyDict_Size(obj), i;
  
  i = size-1;
  while(i >= 0) {
    PyObject *element = PyList_GetItem(pyList, i);
    PyObject *key = PyTuple_GetItem(element, 0),
      *value = PyTuple_GetItem(element, 1);
    ERL_NIF_TERM head = 
      enif_make_tuple(env, 2, 
                      enif_make_atom(env, PyString_AsString(key)), 
                      py_to_erl(env, value));
    
    list = enif_make_list_cell(env, head, list);

    i--;
  };

  return list;
};

ERL_NIF_TERM pytherl_class_to_proplist(ErlNifEnv *env, PyObject *obj) {
  PyRun_SimpleString("result = dict((name, getattr(result, name)) for name in dir(result) if not name.startswith('__'))");
  
  return py_to_erl(env, pytherl_result());
};

ERL_NIF_TERM py_to_erl(ErlNifEnv *env, PyObject *pyObj) {
    if(PyInt_Check(pyObj)) {
        return enif_make_int(env, (int)PyInt_AS_LONG(pyObj));
    } else if(PyBool_Check(pyObj)) {
        return enif_make_atom(env, Py_True == pyObj ? "true" : "false");
    } else if(PyString_Check(pyObj)) {
        return enif_make_string(env, PyString_AsString(pyObj), ERL_NIF_LATIN1);
    } else if(PyList_Check(pyObj)) {
        return pytherl_make_list(env, pyObj);
    } else if(PyInstance_Check(pyObj)) {
        return pytherl_class_to_proplist(env, pyObj);
    } else if(PyDict_Check(pyObj)) {
        return pytherl_make_proplist(env, pyObj);
    } else if(PyTuple_Check(pyObj)) {
        return pytherl_make_tuple(env, pyObj);
    };
    
    return enif_make_badarg(env);
};
