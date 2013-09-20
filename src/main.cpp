#include <cstdlib>

#include "../inc/TraceFileHandler.h"
#include "../inc/SearchEngine.h"
#include "../inc/TileAccessAnalyzer.h"

using namespace std;

int main(int argc, char** argv) {
	string traceFileName(argv[1]);
	Int numOfFrames = atoi(argv[2]);
	Int mode = atoi(argv[3]);
	
	TraceFileHandler* tfh = new TraceFileHandler(traceFileName, numOfFrames);	
	if(mode == 0) {
		TileAccessAnalyzer* taa = new TileAccessAnalyzer(tfh);
		taa->analyze();
	}
	else {
		MemPredictionHandler::init(tfh);
		SearchEngine* se = new SearchEngine(tfh);
		se->search();

		MemPredictionHandler::fp.close();
		MemPredictionHandler::fp1.close();
	}
	
	return 0;
}

