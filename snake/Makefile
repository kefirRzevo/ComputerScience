CC  = gcc
CXX = g++ -std=c++11

CXXFLAGS = -I inc -Wall -Wextra -g -std=c++14 #-Wall -Wextra \
-Weffc++ -Wc++0x-compat \
-Wc++11-compat -Wc++14-compat -Wcast-qual \
-Wchar-subscripts   \
-Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security  \
-Wformat=2 -Winline \
-Wmissing-declarations -Wnon-virtual-dtor \
-Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls \
-Wshadow -Wsign-conversion -Wsign-promo \
 -Wstrict-overflow=2 \
 -Wsuggest-override \
 -Wundef -Wunreachable-code -Wunused \
 -Wvariadic-macros \
-Wno-missing-field-initializers -Wno-narrowing \
-Wno-old-style-cast -Wno-varargs -fcheck-new \
-fsized-deallocation -fstack-check -fstack-protector \
-fstrict-overflow \
-fno-omit-frame-pointer \
-fsanitize=address \
-fsanitize=bool \
-fsanitize=bounds \
-fsanitize=enum \
-fsanitize=float-cast-overflow \
-fsanitize=float-divide-by-zero \
-fsanitize=integer-divide-by-zero \
-fsanitize=nonnull-attribute \
-fsanitize=null \
-fsanitize=return \
-fsanitize=returns-nonnull-attribute \
-fsanitize=shift \
-fsanitize=signed-integer-overflow \
-fsanitize=undefined \
-fsanitize=unreachable \
-fsanitize=vla-bound \
-fsanitize=vptr
DBGFLAGS = -fsanitize=address -ggdb3

TARGET = snake
SRCDIR = src/
OBJDIR = build/
INCDIR = include/

SOURCES = main.cpp $(wildcard $(SRCDIR)*.cpp)
OBJECTS = $(foreach SRC, $(SOURCES), $(basename $(OBJDIR)$(notdir $(SRC))).o)
DEPENDS = $(patsubst %.o, %.d, $(OBJECTS))

# .PHONY means these rules get executed even if
# files of those names exist.

all: build #dirs

build: $(TARGET)

.PHONY: clean dirs

dirs:
	@ mkdir -p $(OBJDIR)
	@ mkdir -p $(SRCDIR)
	@ mkdir -p $(INCDIR)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system

-include $(DEPENDS)

$(OBJDIR)main.o: main.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@


#define BuildRule =
#$(OBJDIR)$(basename $(notdir $(1))).o: $(1)
#	echo $@
#	$(CXX) $< -c -o $@ $(CXXFLAGS)
#endef

#-include depends

#$(eval $(call BuildRule,main))

#$(foreach SRC, $(SOURCES), $(eval $(call BuildRule, $(SRC))))
