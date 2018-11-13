#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

char chrom[5];
int start;
int stop;
float map;

class binEntry
{ public:
	int start;
	int stop;
	float map;

	binEntry()
	{
		start = -1;
		stop = -1;
		map = -1;
	}
	
	binEntry(int startInp, int stopInp, float mapInp)
	{
		start = startInp;
		stop = stopInp;
		map = mapInp;
	}
};

float  getWeightedAverage(binEntry *entries, int numEntries)
{
	int i = 0;
	float sum = 0.0;
	
	for (i = 0; i < numEntries; ++i) {
		binEntry curEntry = entries[i];
		sum += curEntry.map * (curEntry.stop - curEntry.start) / 5000.0;
	}
	return sum;
}

int main(int argc, char** argv)
{

	if(argc < 2)
	{
		cout << "Please pass an input file location." << endl;
		return 1;
	}
	std::ifstream infile(argv[1]);

	int firstBin = 0;
	float curSum = 0;	
	binEntry curEntries[5000]; // worst case, we'll have 1 range for every bp in bin.
	int numEntries = 0; // keep track of how many entries we have.
	while (infile >> chrom >> start >> stop >> map)
	{
		int roundedStart = floor(start / 5000) * 5000;
		int roundedEnd = floor(stop / 5000) * 5000;

		// if firstBin < rounded start, that means our current bin is before the next
		// range's start, aka we are moving onto the next bin.
		// save any data we have so far and start incrementing to find the right bin.
		if (firstBin < roundedStart)
		{
			if( numEntries > 0)
			{
				float avgMap = getWeightedAverage(curEntries, numEntries);
				cout << firstBin << "	" << avgMap << endl;
				numEntries = 0;
				firstBin += 5000;
			}
			while(firstBin < roundedStart)
			{
				//cout << firstBin << " " << 0 << endl;
				firstBin += 5000;
			}
		}

		// we've found the right bin now.

		// find the minimum start, for later knowledge.
		int min = 0;
		if(start > firstBin)
		{
			min = start; // this range starts within this bin
		}
		else
		{
			min = firstBin;	// range started before this bin.
		}

		int end = 0;
		// if this range ends within this bin...
		if(roundedEnd == firstBin)
		{
			end = stop;
			binEntry entry(min, end, map);
			curEntries[numEntries] = entry;
			numEntries += 1;
		}
		else
		{
			while(roundedEnd > firstBin)
			{
				end = firstBin + 5000;
				binEntry entry(min, end, map);
				curEntries[numEntries] = entry;
				numEntries += 1;
				// & do completion logic.
				float avgMap = getWeightedAverage(curEntries, numEntries);
				cout << firstBin << "	" << avgMap << endl;

				firstBin += 5000;
				numEntries = 0;
				min = firstBin;
			}
			end = stop;
			binEntry entry(min, end, map);
			curEntries[numEntries] = entry;
			numEntries += 1;
		}

	}
	if(numEntries > 0)
	{
		float avgMap = getWeightedAverage(curEntries, numEntries);
		cout << firstBin << "	" << avgMap << endl;
	}
	return 0;
}
