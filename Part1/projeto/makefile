SDIR=./src
IDIR=./header
ODIR=./obj
CC=gcc

IFLAGS	=-I$(IDIR)
CFLAGS=-g -Wall

TRANSMITTER_O_FILES=$(ODIR)/transmitter.o
RECEIVER_O_FILES   =$(ODIR)/receiver.o

O_FILES=$(ODIR)/ll.o $(ODIR)/app.o $(ODIR)/config.o $(ODIR)/com.o $(ODIR)/state_machine.o $(ODIR)/utils.o

all: transmitter receiver

transmitter: $(TRANSMITTER_O_FILES) $(O_FILES)
	$(CC) $^ -o $@

receiver: $(RECEIVER_O_FILES) $(O_FILES)
	$(CC) $^ -o $@

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean: 
	rm -f transmitter receiver
	rm -rf $(ODIR)