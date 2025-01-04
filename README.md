# CyclicRedundancyCheck
[CSE4175 Computer Networks mp1 ]

g++ -o crc_encoder_20200001 crc_encoder_20200001.cc
g++ -o crc_decoder_20200001 crc_decoder_20200001.cc

./crc_encoder input_file output_file generator dataword_size
./crc_decoder input_file output_file result_file generator dataword_size
