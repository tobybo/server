
ifeq ($(DEBUG),true)
cc = g++ -std=c++11 -g
ccc = gcc -g
VERSION = debug
else
cc = g++ -std=c++11
ccc = gcc
VERSION = release
endif

SRCSC = $(wildcard *.c)
SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o) $(SRCSC:.c=.o)
DEPS = $(SRCS:.cpp=.d) $(SRCSC:.c=.d)

BIN := $(addprefix $(BUILD_ROOT)/,$(BIN))

LINK_OBJ_DIR = $(BUILD_ROOT)/bin/link_obj
DEP_DIR = $(BUILD_ROOT)/bin/dep

$(shell mkdir -p $(LINK_OBJ_DIR))
$(shell mkdir -p $(DEP_DIR))

OBJS := $(addprefix $(LINK_OBJ_DIR)/,$(OBJS))
DEPS := $(addprefix $(DEP_DIR)/,$(DEPS))

LINK_OBJ = $(wildcard $(LINK_OBJ_DIR)/*.o)
LINK_OBJ += $(OBJS)

all:$(DEPS) $(OBJS) $(BIN)

ifneq ("$(wildcard $(DEPS))","")
include $(DEPS)
endif

# .bin:.o
$(BIN):$(LINK_OBJ)
	#@echo "do: bin:linkobj $(LINK_OBJ)"
	@echo "--------------build $(VERSION) mode-------------------"
	$(cc) -o $@ $^ $(LIB_NAME)

# .o:.cpp
$(LINK_OBJ_DIR)/%.o:%.cpp
	@echo "do: obj:cpp"
	#$(cc) -I$(INCLUDE_PATH) -I /usr/include/mysql/ -o $@ -c $(filter %.cpp,$^)
	$(cc) $(INCLUDE_PATH) -o $@ -c $(filter %.cpp,$^)

$(DEP_DIR)/%.d:%.cpp
	@echo "do: dep:cpp"
	#echo -n $(LINK_OBJ_DIR)/ > $@
	#$(cc) -I$(INCLUDE_PATH) -I /usr/include/mysql/ -MM $^ >> $@
	$(cc) $(INCLUDE_PATH) -MM $^ >> $@

# .o:.cpp
$(LINK_OBJ_DIR)/%.o:%.c
	@echo "do: obj:c"
	#$(cc) -I$(INCLUDE_PATH) -I /usr/include/mysql/ -o $@ -c $(filter %.cpp,$^)
	$(ccc) $(INCLUDE_PATH) -o $@ -c $(filter %.c,$^)

$(DEP_DIR)/%.d:%.c
	@echo "do: dep:c"
	#echo -n $(LINK_OBJ_DIR)/ > $@
	#$(cc) -I$(INCLUDE_PATH) -I /usr/include/mysql/ -MM $^ >> $@
	$(ccc) $(INCLUDE_PATH) -MM $^ >> $@
