#include "cacheMem.h"
#include "cacheBlock.h"
//#include "common.h"

void cacheBlock::validate(uint32_t m_address)
{
	address = m_address;
	validated = true;
	state = 'S';
	//cout << "validou-se address: " << address<<endl;
}

void cacheBlock::setStateBlock(char new_state)
{
	state = new_state;
}

bool cacheBlock::checkValidation(uint32_t m_address)
{
	if((m_address == address) && state != 'I'){
		return true;
	}
	return false;
}


void cacheBlock::invalidate()
{
	validated = false;
	state = 'I';
}


cacheBlock::cacheBlock()
{
	address = -1;
	validated = false;
	state = 'I';
}

