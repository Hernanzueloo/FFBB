# Version 3.0
CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi
LDLIBS = -lodbc

# recompile if this heder changes


export PGDATABASE:=classicmodels
export PGUSER :=alumnodb
export PGPASSWORD :=alumnodb
export PGCLIENTENCODING:=LATIN9
export PGHOST:=localhost

HEADERS = odbc.h funciones.h products.h orders.h customers.h
DEPS = odbc.o funciones.o products.o customers.o orders.o

EXE = menu
OBJ = $(EXE).o


DBNAME =$(PGDATABASE)
PSQL = psql
CREATEDB = createdb
DROPDB = dropdb --if-exists
PG_DUMP = pg_dump
PG_RESTORE = pg_restore

all: dropdb createdb restore shell
compile : $(EXE)

createdb:
	@echo Creando BBDD
	@$(CREATEDB) $(DBNAME)
dropdb:
	@echo Eliminando BBDD
	@$(DROPDB) $(DBNAME)
	rm -f *.log
dump:
	@echo creando dumpfile
	@$(PG_DUMP) > $(DBNAME).sql
restore:
	@echo restore data base
	@cat $(DBNAME).sql | $(PSQL)  
psql: shell
shell:
	@echo create psql shell
	@$(PSQL)  





%.o: %.c $(HEADERS) 
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $< 

$(EXE): $(DEPS) $(OBJ) 
	$(CC) -o $(EXE) $(OBJ) $(DEPS) $(LDLIBS)

clean :
	rm -f *.o core $(EXE)