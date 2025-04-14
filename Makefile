CXX = g++

FLAGS = -MMD -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations            \
-Wcast-align -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal			         \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual     \
-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn \
-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code 			     \
-Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs        \
-Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer 			     \
-Wlarger-than=327678 -Wstack-usage=8192 -pie -fPIE -Werror=vla 																					     \
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

SRC_DIR  = src
INC_DIR  = inc
OBJ_DIR  = obj
DEP_DIR  = dep

LIBS = -lblake3 -lisal

TEMP_DIR = $(OBJ_DIR) $(DEP_DIR)

EXEC 	 = file_indexer

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: build
build: $(EXEC)

$(EXEC): $(OBJ)
	@$(CXX) $(FLAGS) $(OBJ) -o $(EXEC) $(LIBS)

$(OBJ): DEP = $(patsubst $(OBJ_DIR)/%.o, $(DEP_DIR)/%.d, $@)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp | $(TEMP_DIR)
	@$(CXX) -I $(INC_DIR) -MF $(DEP) -o $@ -c $< $(FLAGS)

$(TEMP_DIR):
	@mkdir -p $@

.PHONY: run
run:
	@./$(EXEC)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) file_indexer $(DEP_DIR)

-include $(DEP)/*.d