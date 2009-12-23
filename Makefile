ERL_INCLUDE=/usr/local/erlang/r13b03/lib/erlang/usr/include
PYTHON_INCLUDE=/usr/include/python2.6/

INCLUDES= -I$(ERL_INCLUDE) -I$(PYTHON_INCLUDE)

GCC_FLAGS=-fPIC -shared -Wall

CFLAGS=$(GCC_FLAGS) $(INCLUDES)

SRC_CDIR=c_src/
SRC_ERLDIR=src/
CDIR=priv/
ERLDIR=ebin/

C_FILES=$(wildcard $(SRC_CDIR)*c)
DRIVER=$(CDIR)pytherl.so
BEAM=$(ERLDIR)pytherl.beam

all: $(DRIVER) $(BEAM)

clean:
	rm -f $(CDIR)*o $(DRIVER) $(ERLDIR)*beam

$(ERLDIR)%.beam: $(SRC_ERLDIR)%.erl
	@echo ERLC $<
	@erlc $(ERLCFLAGS) -o $(ERLDIR) $<

$(DRIVER): $(C_FILES)
	@echo GCC $<
	@gcc -o $@ $(CFLAGS) $<