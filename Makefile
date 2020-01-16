#-------------------------------------------------------------------------
###########################
# Sentencia de Constantes #
###########################

# --- opciones de ejecución ---
CLEAN_COMMAND := rm
CC := gcc
EXE_NAME := lab.out
DEBUG_MODE :=
EXE_NAME_DEBUG := lab_debug.out
EXE_NAME_CHILD := vis.out

# --- Carpetas ---
INCLUDE_DIR := incl/# contiene los headers (.h)
SRC := src/# contiene los .c con codigo tradicional
OBJ := obj/# contiene los object (.o) resultantes de compilacion -c

# --- colores ---

NO_COLOR= \033[0m
OK_COLOR= \033[32;01m
ERROR_COLOR= \033[31;01m
WARN_COLOR= \033[33;01m
SUSF_PRINT = \033[1;34m
PUR_COLOR = \033[0;35m

#-------------------------------------------------------------------------
########################
# Sentencia de Recetas #
########################

SOURCES := $(wildcard $(SRC)*.c)
OBJECTS := $(patsubst $(SRC)%.c, $(OBJ)%.o, $(SOURCES))

all: clean main
	
	@echo "$(PUR_COLOR)Ejecutable generado:$(NO_COLOR) $(OK_COLOR)$(EXE_NAME)$(NO_COLOR) "
	@echo 

main: $(OBJECTS)
	@echo "Se genera nuevo $(WARN_COLOR).o$(NO_COLOR) y $(WARN_COLOR).out$(NO_COLOR)"
	($(CC) $^ -g -lm -L$(INCLUDE_DIR) -pthread -lpng $(DEBUG_MODE) -o $(EXE_NAME)) \
		||  (echo "$(ERROR_COLOR)[ERROR]$(NO_COLOR)" && exit 1; )
	@echo 
	

debug: set-debug

set-debug: DEBUG_MODE := -DDEBUG
set-debug: all-debug

all-debug: clean main-debug
	@echo "$(ERROR_COLOR)[DEBUG MODE] $(PUR_COLOR)Ejecutable generado!$(NO_COLOR) Nombre: $(OK_COLOR)$(EXE_NAME_DEBUG)$(NO_COLOR) "
	@echo "\n"

main-debug: $(OBJECTS)
	@echo "Generando ejecutable ..."
	($(CC) $^ -lm $(DEBUG_MODE) -o $(EXE_NAME_DEBUG) && echo "$(OK_COLOR)[OK]$(NO_COLOR)") \
		||  (echo "$(ERROR_COLOR)[ERROR]$(NO_COLOR)" && exit 1; )
	@echo "\n"

$(OBJ)%.o: $(SRC)%.c
	($(CC) $(DEBUG_MODE) -g -lm -lpthread -I$(SRC) -c $< -o $@) \
		||  (echo "$(ERROR_COLOR)[ERROR]$(NO_COLOR)" && exit 1; )
	
clean: 
	
	@echo 
	@echo "Eliminado antiguo $(WARN_COLOR).o$(NO_COLOR) y $(WARN_COLOR).out$(NO_COLOR)"
	@echo >> rm.out
	($(CLEAN_COMMAND) *.out) \
		||  (echo "$(ERROR_COLOR)[ERROR]$(NO_COLOR)" && exit 1; ) 
	@echo >> $(OBJ)rm.o
	($(CLEAN_COMMAND) $(OBJ)*.o) \
		||  (echo "$(ERROR_COLOR)[ERROR]$(NO_COLOR)" && exit 1; )
	@echo

.SILENT: clean all make main $(OBJ)%.o $(SOURCES) $(OBJECTS) main-child $(SRC)%.c main-debug