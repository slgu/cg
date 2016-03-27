all:
	g++ -O2 -std=c++0x -g *.cpp -I. -I/usr/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o prog_out
mac:
	g++ -O2 -std=c++0x -g *.cpp -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -o prog_out
tbb:
	g++ -stdlib=libc++ -O2 -DUSE_TBB -std=c++11 -g *.cpp -I/usr/local/Cellar/tbb/4.4-20150728/include -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -ltbb -Wall -o prog_out
glossy:
	g++ -O2 -DUSE_GLOSSY -DUSE_TBB -std=c++0x -g *.cpp -I/usr/local/Cellar/tbb/4.4-20150728/include -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -ltbb -Wall -o prog_out
clean:
	rm -rf prog_out prog_out.dSYM
