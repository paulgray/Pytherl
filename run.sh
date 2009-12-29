#!/bin/sh

PYTHONPATH=`pwd`:$PYTHONPATH erl -pa ebin -eval "pytherl:init()."
