CC  = gcc
CXX = g++ -std=c++17

CXXFLAGS = -I inc -Wall -Wextra -g -std=c++17 #-Wall -Wextra \
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
LDFLAGS  = -ldl

TARGET = app


SRCDIR = graphlib/ ./ application/ widgets/ plugin/ gui/ config/
OBJDIR = build/
INCDIR = application/ graphlib/ widgets/ plugin/ gui/ config/
LIBDIR = plugins/

SOURCES = $(foreach SRC, $(SRCDIR), $(wildcard $(SRC)*.cpp))
OBJECTS = $(foreach SRC, $(SOURCES), $(basename $(OBJDIR)$(SRC)).o)
DEPENDS = $(patsubst %.o, %.d, $(OBJECTS))
LIB     = $(patsubst %.cpp, $(OBJDIR)%.so, $(wildcard $(LIBDIR)*.cpp))


all: dirs plugins build

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(TARGET) $(OBJDIR)$(LIB)


plugins: $(LIB)

$(OBJDIR)%.so: %.cpp
	$(CXX) -shared -fPIC -o $@ $<

.PHONY: clean dirs plugins

dirs:
	@ mkdir -p $(OBJDIR)
	@ mkdir -p $(addprefix $(OBJDIR), $(SRCDIR))
	@ mkdir -p $(addprefix $(OBJDIR), $(INCDIR))
	@ mkdir -p $(addprefix $(OBJDIR), $(LIBDIR))

build: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system

-include $(DEPENDS)

$(OBJDIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
