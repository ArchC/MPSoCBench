#ifndef DIRETORIO_H
#define DIRETORIO_H
#include "cacheMem.h"
//#include "common.h"

class Diretorio
{
	public:
		fstream log;
		int totalOfCaches, index_size, started;
		cacheMem cacheMemVector[64];
		int cacheMod[65];
		Diretorio();
		Diretorio(int p){totalOfCaches=p;}
		virtual ~Diretorio();
		void start(int, int);
		bool checkValidation(int, uint32_t, int);
		void checkNumberCache(int);
		bool validate(int, uint32_t, int);
		void unvalidate(int, uint32_t, int);

	protected:
	private:
};

#endif // DIRETORIO_H
