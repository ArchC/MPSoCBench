

#include "Diretorio.h"


Diretorio::Diretorio()
{
	started=false;
	totalOfCaches=MAX_NUMBER_OF_CACHES;
	index_size = MAX_INDEX_SIZE;
	// log.open("dirLog.txt", fstream::out);
	for(int i=0; i<totalOfCaches;i++){
		cacheMemVector[i].numberCache = i;
	}
	//start(2, index_size);

}

void Diretorio::start(int index, int nWay)
{
	if(started) return;

	started=true;
	index_size = index;
	for(int i=0; i<totalOfCaches;i++){
		cacheMemVector[i].nWay = nWay;
		cacheMemVector[i].alocate_blocks(index_size);
	}
	//printf("\nstartando diretorio index = %d, assoc=%d", index, nWay);

}

Diretorio::~Diretorio()
{
	//log.close();
}



bool Diretorio::validate(int numberCache, uint32_t address, int cacheAddress)
{
	
	cacheMemVector[numberCache].validate(address, cacheAddress);
	return true;

}


bool Diretorio::checkValidation(int numberCache, uint32_t address, int cacheIndex)
{
	if(cacheMemVector[numberCache].checkValidation(address, cacheIndex))
	{
		return true;
	}
	return false;
}

void Diretorio::unvalidate(int numberCache, uint32_t address, int cacheBlockIndex)
{

	for(int i=0; i<totalOfCaches;i++)
		if(cacheMemVector[i].numberCache != numberCache)
		{
			cacheMemVector[i].invalidate(address, cacheBlockIndex);
		}
}


void Diretorio::checkNumberCache(int numberCache)
{
	for(int i=0; i<totalOfCaches;i++)
	{
		if(numberCache == cacheMemVector[i].numberCache)
			return;
	}
}
