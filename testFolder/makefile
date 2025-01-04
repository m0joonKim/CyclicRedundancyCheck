CC=g++
CFLAGS = -Wall -Werror
all: crc_decoder crc_encoder

crc_decoder: crc_decoder_20201558.cc
	$(CC) $(CFLAGS) -o crc_decoder crc_decoder_20201558.cc

crc_encoder: crc_encoder_20201558.cc
	$(CC) $(CFLAGS) -o crc_encoder crc_encoder_20201558.cc

clean:
	rm -f crc_decoder crc_encoder
	rm codedstream.rx
	rm codedstream.tx
	rm datastream.rx
	rm result.txt
