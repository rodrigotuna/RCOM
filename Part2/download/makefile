SDIR=./src
IDIR=./header
ODIR=./obj
CC=gcc

IFLAGS	=-I$(IDIR)
CFLAGS=-g -Wall

DOWNLOAD_O_FILES=$(ODIR)/download.o

O_FILES=$(ODIR)/url.o $(ODIR)/client.o

all: download

download: $(DOWNLOAD_O_FILES) $(O_FILES)
	$(CC) $^ -o $@

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean: 
	rm -f download
	rm -rf $(ODIR)