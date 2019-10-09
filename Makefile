
EXCEC = prg 

SRCDIR  = src 
OBJDIR  = obj

SRCS    := $(shell find $(SRCDIR) -name '*.c')
HDRS    := $(shell find $(SRCDIR) -name '*.h')
SRCDIRS := $(shell find . -name '*.c' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

CFLAGS = -Wall

LDFLAGS =


all: $(EXCEC)

$(EXCEC) : buildrepo $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

$(OBJDIR)/%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean: clen
	$(RM) $(EXCEC)
	
buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef
