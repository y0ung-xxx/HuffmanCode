all : archive extract cal

archive : Compressor.cpp
	g++ Compressor.cpp -o archive

extract : Decompressor.cpp
	g++ Decompressor.cpp -o extract

calc_rate : calc_rate.cpp
	g++ cal.cpp -o cal

clean :
	@rm -f archive
	@rm -f extract
	@rm -f cal

.PHONY : all clean
