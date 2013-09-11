#include <cstdlib>

#include "../inc/TraceFileHandler.h"
#include "../inc/TileAccessAnalyzer.h"

using namespace std;

int main(int argc, char** argv) {
	string traceFileName(argv[1]);
	
	TraceFileHandler* tfh = new TraceFileHandler(traceFileName);	
	TileAccessAnalyzer* taa = new TileAccessAnalyzer(tfh);
	taa->analyze();
	
	return 0;
}

