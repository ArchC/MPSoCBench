#include "cacheMem.h"
#include "cacheBlock.h"


cacheMem::~cacheMem()
{
	delete blocks;
}


void cacheMem::alocate_blocks(int index_size)
{
	blocks = new cacheBlock[nWay*index_size];
}


/** @brief ~cacheMem
  *
  * @todo: document this function
  */


/** @brief unvalidate
  *
  * @todo: document this function
  */



/** @brief checkValidation
  *
  * @todo: document this function
  */

bool cacheMem::validate(uint32_t address, int index)
{
	if(index>MAXBLOCKS)
		return false;
	blocks[index].validate(address);
	return true;
}
bool cacheMem::checkValidation(uint32_t address, int cacheIndex)
{
	if(blocks[cacheIndex].address == address)
		return blocks[cacheIndex].checkValidation(address);
	return false;
}

bool cacheMem::invalidate(uint32_t address, int cacheBlockIndex)
{

	for(int i=0; i<nWay;i++)
	{
		if(blocks[cacheBlockIndex+i].address == address && blocks[cacheBlockIndex+i].state != 'I'){
			blocks[cacheBlockIndex+i].invalidate();
			i=nWay;
			return true;
		}
	}
	return false;
}

void cacheMem::writeSetState(uint32_t address, int cacheBlockIndex)
{

	for(int i=0; i<nWay;i++)
	{
		if(blocks[cacheBlockIndex+i].address == address){
			blocks[cacheBlockIndex+i].setStateBlock('M');
			return;
		}
	}

}
bool cacheMem::readSetState(uint32_t address, int cacheBlockIndex)
{

	for(int i=0; i<nWay;i++)
	{
		if(blocks[cacheBlockIndex+i].address == address && blocks[cacheBlockIndex+i].state == 'M'){
			blocks[cacheBlockIndex+i].setStateBlock('S');
			return true;
		}
	}
	return false;

}
/** @brief remove
  *
  * @todo: document this function
  */


/** @brief add
  *
  * @todo: document this function
  */


/** @brief cacheMem
  *
  * @todo: document this function
  */


