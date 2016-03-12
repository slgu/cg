all:
	g++ —O2 -std=c++0x -g *.cpp -I. -I/usr/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o prog_out
mac:
	g++ -std=c++0x -g *.cpp -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o prog_out
clean:
	rm -rf prog_out prog_out.dSYM
