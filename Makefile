all : archive extract calc_rate

archive : Compressor.cpp
	g++ Compressor.cpp -o archive

extract : Decompressor.cpp
	g++ Decompressor.cpp -o extract

calc_rate : calc_rate.cpp
	g++ calc_rate.cpp -o calc_rate

clean :
	@rm -f archive
	@rm -f extract
	@rm -f cal

.PHONY : all clean
