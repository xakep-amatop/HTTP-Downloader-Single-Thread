CC=gcc

CFLAGS=-std=c99 -Wall `pkg-config --cflags libxml-2.0 libcurl`
LIBS=`pkg-config --libs libxml-2.0 libcurl`

OUTPUT=http_download

all:
	$(CC) main.c getXmlFields.c fpath.c download.c $(CFLAGS) $(LIBS) -o $(OUTPUT)
install:
	cp $(OUTPUT) "/usr/bin/"
clean:
	rm -f $(OUTPUT)
