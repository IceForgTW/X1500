
DWCCLASS := $(DWCDIR)/class

SOURCES	+= $(wildcard $(DWCDIR)/*.c)
CFLAGS	+= -I$(DWCDIR)
VPATH   += $(DWCDIR)

SOURCES	+= $(wildcard $(DWCCLASS)/*.c)
VPATH   += $(DWCCLASS)





