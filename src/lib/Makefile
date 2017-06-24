
RDISTDIR ?= $(DISTDIR)

OBJECTS := work.o
DISTS = $(addprefix $(RDISTDIR)/,$(OBJECTS))

TARGET := $(LIBNAME)


$(RDISTDIR)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@ $(LFLAGS)

$(RDISTDIR)/$(TARGET): $(DISTS)
	@echo "Making shared lib: $(TARGET)"
	ar -rcs $@ $^

.PHONY: all
all: $(TARGET)

