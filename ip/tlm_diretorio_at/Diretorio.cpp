#include "Diretorio.h"



Diretorio::Diretorio()
{
	started=0;
	totalOfCaches=64;
	index_size = 1024;
	log.open("dirLog.txt", fstream::out);
	for(int i=0; i<totalOfCaches;i++){
		cacheMemVector[i].numberCache = i;
	}
	start(2, index_size);

}

void Diretorio::start(int n, int index)
{
	if(started != 0)
		return;
	started++;
	index_size = index;
	for(int i=0; i<totalOfCaches;i++){
		cacheMemVector[i].nWay = n;
		cacheMemVector[i].alocate_blocks(index_size);
	}
}

Diretorio::~Diretorio()
{
	log.close();
}



/** @brief validate
  *
  * @todo: document this function
  */
bool Diretorio::validate(int numberCache, uint32_t address, int cacheAddress)
{
	//checkNumberCache(numberCache);
	int n=0;
			cacheMemVector[numberCache].validate(address, cacheAddress);
			for(int i =0;i<totalOfCaches;i++)
			{
				if(i!= numberCache)
				{
					if(cacheMemVector[i].readSetState(address, cacheAddress))
					{
						n++;
						cacheMod[n] = cacheMemVector[i].numberCache;
					}
				}
			}
				//log << "validou-se numberCacheessador " << numberCache << " cacheAddress: " << cacheAddress << " endereço: " << address << endl;
	cacheMod[0] = n;
			return true;

}

/** @brief checkValidation
  *
  * @todo: document this function
  */

bool Diretorio::checkValidation(int numberCache, uint32_t address, int cacheIndex)
{
	//checkNumberCache(numberCache);
			if(cacheMemVector[numberCache].checkValidation(address, cacheIndex))
			{

				//log << "Checagem do numberCacheessador: " << i << " endereço: " << address << " é Valido" <<endl;
				return true;
			}
		//log << "Checagem do numberCacheessador: " << numberCache << " endereço: " << address << " é Invalido" <<endl;
	return false;
}

void Diretorio::unvalidate(int numberCache, uint32_t address, int cacheBlockIndex)
{
	//checkNumberCache(numberCache);
	int n=0;
	for(int i=0; i<totalOfCaches;i++){
		if(cacheMemVector[i].numberCache != numberCache)
		{
			if(cacheMemVector[i].invalidate(address, cacheBlockIndex))
			{
				n++;
				cacheMod[n] = cacheMemVector[i].numberCache;
			}
		}
		else
			cacheMemVector[i].writeSetState(address, cacheBlockIndex);
	}
	cacheMod[0] = n;
}



/** @brief remove
  *
  * @todo: document this function
  */
void Diretorio::checkNumberCache(int numberCache)
{
	for(int i=0; i<totalOfCaches;i++)
	{
		if(numberCache == cacheMemVector[i].numberCache)
			return;
	}
}

/** @brief add
  *
  * @todo: document this function
  */

