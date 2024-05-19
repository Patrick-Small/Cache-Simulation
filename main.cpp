/*
	Patrick Small
	CS_441 Computer Architecture
	04/20/2024

	This C++ Program simulates a Set Associative Cache using LRU Replacement. It takes in
	4 inputs, the first being a trace file that contains memor addresses, and then 3 numbers
	that are specified below. This program can simulate a Direct Mapped cache if you set the
	number of blocks per set to 0, and can also simulate a Fully Associative cache if you
	set the number of direct-mapped sets to 0. 
*/

/*
	Inputs go in this order:
	1. Memory trace input file
	2. X, where 2^X == # of direct-mapped sets
	3. Y, where 2^Y == # of blocks/set
	4. Z, where 2^Z == # of cached words/block
*/


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;


struct Block {
	bool dirty = false;
	uint64_t tag;
	unsigned int LRU = 0;
};

struct Set {
	vector<Block> blocks;
};


//Prototypes
void increaseLRU(vector<Set>&, int);


int main(int argc, char* argv[])
{

	//Check the input #
	if (argc != 5)
	{
		cerr << "Wrong number of inputs/no inputs" << endl;
	}

	//Opening and reading in .tra file
	ifstream f(argv[1]);

	if (!f)
	{
		cerr << "Error: File cannot be opened" << endl;
	}

	//Declarations
	string trace = "";
	int SETS = 1 << atoi(argv[2]);
	int BPS = 1 << atoi(argv[3]);
	int WPB = 1 << atoi(argv[4]);
	uint64_t SETSpow = atoi(argv[2]);
	uint64_t BPSpow = atoi(argv[3]);
	uint64_t WPBpow = atoi(argv[4]);
	uint64_t currSet;
	uint64_t currTag;
	uint64_t addr = 0;
	uint64_t addrCnt = 0;
	uint64_t hitCnt = 0;

	//Create cache
	vector<Set> cache = vector<Set>(SETS);

	for (int i = 0; i < SETS; i++)
	{
		cache[i].blocks = vector<Block>(BPS);
	}

	//Loops until EOF
	while (getline(f, trace))
	{
		bool cached = false;
		if (trace.length() != 0)
		{
			addrCnt++;

			//Convert string to uint64_t
			stringstream str(trace);
			str >> hex >> addr;

			//Gather information for the set and tag
			addr >>= WPBpow;
			currSet = addr & ~(~0U << SETSpow);
			currTag = addr >> SETSpow;

			//Look through the cache
			for (int i = 0; i < BPS; i++)
			{
				if (!cached)
				{
					//If there is a block that is clean
					if (!cache[currSet].blocks[i].dirty)
					{
						//Insert the tag into the clean block
						cache[currSet].blocks[i].tag = currTag;
						cache[currSet].blocks[i].dirty = true;
						increaseLRU(cache, BPS);
						cache[currSet].blocks[i].LRU = 0;
						cached = true;
					}
					//If there is a hit
					else if (cache[currSet].blocks[i].tag == currTag)
					{
						increaseLRU(cache, BPS);
						cache[currSet].blocks[i].LRU = 0;
						hitCnt++;
						cached = true;
					}
				}
			}

			//If it wasn't cached, then store using LRU rules
			if (!cached)
			{
				int largestLRU = 0;
				int oldest = 0;

				//Go through blocks
				for (int i = 0; i < BPS; i++)
				{
					if (cache[currSet].blocks[i].LRU > largestLRU)
					{
						largestLRU = cache[currSet].blocks[i].LRU;
						oldest = i;
					}
				}

				cache[currSet].blocks[oldest].tag = currTag;
				increaseLRU(cache, BPS);
				cache[currSet].blocks[oldest].LRU = 0;
			}

		}

	}
	cout << "hits: " << hitCnt << "/" << addrCnt << endl;

	return 0;
}

void increaseLRU(vector<Set>& cache, int blocks)
{
	for (int i = 0; i < cache.size(); i++)
	{
		for (int j = 0; j < blocks; j++)
		{
			if (cache[i].blocks[j].dirty)
			{
				cache[i].blocks[j].LRU++;
			}
		}
	}
}