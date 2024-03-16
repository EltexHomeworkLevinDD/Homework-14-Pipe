CC:=gcc
CFLAGS:=-g

UTILS_D:=./utils
UTILS_S_D:=$(UTILS_D)/src
UTILS_O_D:=$(UTILS_D)/obj
UTILS_B_D:=$(UTILS_D)/bin

UTILS_LIST:= showdir

.PHONY: create_utils_dirs all clean

# $(1) - util_list; $(2) - util_src; $(3) - util_obj; $(4) - util_bin

# create BIN dir for all
# create OBJ dir for each
create_utils_dirs:
	@mkdir -p $(UTILS_B_D) $(UTILS_O_D); \
	for i in $(UTILS_LIST); do \
		mkdir -p $(UTILS_O_D)/$$i ; \
	done

# $ - используется для обращения к переменным
# $$ - 
# ; - используется для разделения команд в одной строке
# \ - используется для указания на продолжение команды на следующей строке
# @ в начале - не выводить на экран (используется только перед первой строкой)

# for i in $(UTILS_LIST); do \ ... done - это цикл for в оболочке Bash, 
#     который проходит по каждому элементу из переменной $(UTILS_LIST)
# $$i используется для обращения к переменной i внутри цикла. Двойной символ 
#     доллара $$ используется в Makefile для обращения к переменным оболочки 
#     Bash внутри правил Makefile.
# $(UTILS_O_D) - пременная Makefile, значит $
# i - переменная Bash, значит $$

# Получить директории obj, src, bin выбранной $(1) утилиты
# put $(1) - util
# _UTIL_SRC_D - source dir for this util
# _UTIL_OBJ_D - object dir for this util
# _UTIL_BIN_D - binary dir for this util

define get_util_dirs_files
	_UTIL_SRC_D := $(UTILS_S_D)/$(1)
	_UTIL_OBJ_D := $(UTILS_O_D)/$(1)
	_UTIL_BIN_D := $(UTILS_B_D)
	_UTIL_SRC_FILES := $(wildcard $(_UTIL_SRC_D)/*.c)
	_UTIL_OBJ_FILES := $(patsubst $(_UTIL_SRC_D)/%.c,$(_UTIL_OBJ_D)/%.o,$(_UTIL_SRC_FILES))
endef

$(eval $(call get_util_dirs_files,showdir))
showdir: $(_UTIL_SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(_UTIL_OBJ_D)/%.o: $(_UTIL_SRC_D)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: create_utils_dirs showdir






