/***************************************************************************
SHA Secure Hash Algorithm				                                
*															Parallel 1.0 Version															
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:					Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:				4.2.4								
* Environment: 				Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	27th October 2009					
* End Date:         	23th November 2009			
*		
*********************************************************************************/



/* NIST Secure Hash Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//#include <pthread.h>
#include "sha.h"


#define MAX_NO_FILES 16
#define MAX_WORKERS 16
#define PRINT 1

int no_files;// = MAX_NO_FILES;    // this value can be modified but will interfere with the partitioning
int no_workers;  		// this value is an argument (is the same as the number of processors in the platform)
				// it can't be modified directly 

//FILE *fileInArray[MAX_NO_FILES];

/******************************************************
acPthread
*******************************************************/

#ifndef acPthread_H_
#include "../acPthread_p.h"
#endif

#define malloc acPthread_malloc
#define realloc acPthread_realloc
#define free acPthread_free
#define calloc acPthread_calloc

extern pthread_mutex_t mutex_print;
extern int pthread_threads_per_software; // number of threads per software 

extern FILE *fileout_sha;

/*extern FILE *filein0_sha;
extern FILE *filein1_sha;
extern FILE *filein2_sha;
extern FILE *filein3_sha;
extern FILE *filein4_sha;
extern FILE *filein5_sha;
extern FILE *filein6_sha;
extern FILE *filein7_sha;
extern FILE *filein8_sha;
extern FILE *filein9_sha;
extern FILE *filein10_sha;
extern FILE *filein11_sha;
extern FILE *filein12_sha;
extern FILE *filein13_sha;
extern FILE *filein14_sha;
extern FILE *filein15_sha;
*/

int main_sha()
{

	
/*	fileInArray[0]=filein0_sha;
	fileInArray[1]=filein1_sha;
	fileInArray[2]=filein2_sha;
	fileInArray[3]=filein3_sha;
	fileInArray[4]=filein4_sha;
	fileInArray[5]=filein5_sha;
	fileInArray[6]=filein6_sha;
	fileInArray[7]=filein7_sha;
	fileInArray[8]=filein8_sha;
	fileInArray[9]=filein9_sha;
	fileInArray[10]=filein10_sha;
	fileInArray[11]=filein11_sha;
	fileInArray[12]=filein12_sha;
	fileInArray[13]=filein13_sha;
	fileInArray[14]=filein14_sha;
	fileInArray[15]=filein15_sha;
*/
		
	
	/* NUMBER OF WORKER THREADS */
		
        no_workers = pthread_threads_per_software;
	no_files = pthread_threads_per_software;
	
	/* ARGUMENTS */
	int m_argc;
	char *m_argv[3];

	m_argc = 3; //./sha -P -2
	m_argv[1] = "-P";  // or -S without argv[2]
	m_argv[2] = "-";    
        

	
				
	main0_sha(m_argc,m_argv);

	fclose(fileout_sha);

/*	fclose(filein0_sha);
	fclose(filein1_sha);
	fclose(filein2_sha);
	fclose(filein3_sha);
	fclose(filein4_sha);
	fclose(filein5_sha);
	fclose(filein6_sha);
	fclose(filein7_sha);
	fclose(filein8_sha);
	fclose(filein9_sha);
	fclose(filein10_sha);
	fclose(filein11_sha);
	fclose(filein12_sha);
	fclose(filein13_sha);
	fclose(filein14_sha);
	fclose(filein15_sha);
*/

  	
	return 0; 
}
/******************************************************
acPthread
*******************************************************/



typedef struct Params {
	int s_index;
	int e_index;
	int no;
	char *name;
} parameters;

struct fileData{
	  BYTE *data;;
	  int size;
}Tdata[MAX_NO_FILES];


/*struct Tinputs{
	BYTE *data;
	int size;
}inputs[MAX_WORKERS];*/


//char* in_file_list[MAX_NO_FILES]={"in_file0.asc", "in_file1.asc", "in_file2.asc","in_file3.asc","in_file4.asc","in_file5.asc","in_file6.asc","in_file7.asc","in_file8.asc","in_file9.asc","in_file10.asc","in_file11.asc","in_file12.asc","in_file13.asc","in_file14.asc","in_file15.asc"};


pthread_attr_t sha_attr;
pthread_mutex_t string_mutex;
pthread_t workers[MAX_WORKERS]; 

static int partition_size;
static parameters paramsArr[MAX_WORKERS];

void readFilesData(){

	Tdata[0].data = "rereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskj";	
	Tdata[0].size = 1024;


	Tdata[1].data = "lerereliabletensethisadvicenowEnjoythepoweraneautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[1].size = 1024;


	Tdata[2].data = "eanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnoturereliabletensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownmnderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[2].size = 1024;

	Tdata[3].data = "autyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownmnderstandeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnoturereliabletensethisadvicenowEnjoythepowerandbethepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[3].size = 1024;

	Tdata[4].data = "litylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldispautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownmnderstandeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnoturereliabletensethisadvicenowEnjoythepowerandbethepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibi";	
	Tdata[4].size = 1024;


	Tdata[5].data = "spensethisadvicenowEnjoythepowerarereliablethanmyownmeanderingexperienceIwilldindbeautyofyouryouthOhnutyofyouryouthuntiltheyvefadedButtrustmein20yeaevermindYouwillnotunderstandthepowerandbearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskj";	
	Tdata[5].size = 1024;


	Tdata[6].data = "meanderingexperienceIwilldispensrereliabletensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[6].size = 1024;


	Tdata[7].data = "eanderingexjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownmnderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilityperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnoturereliabletensethisadvicenowEnlaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[7].size = 1024;


	Tdata[8].data = "ienceIwilldispensethisadvicenowEnautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownmnderstandeanderingexperjoletensethisadvicenowEnjoythepowerandbethepowerythepowerandbeautyofyouryouthOhnevermindYouwillnoturereliabandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[8].size = 1024;


	Tdata[9].data = "isadvicenowEnjoythrereliablethanmyownmeanderingexperienceIwilldispensethepowerandbeautyofyouryouthOhneandbeautyofyouryouthuntiltheyvefvermindYouwillnotunderstandthepoweradedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskj";	
	Tdata[9].size = 1024;


	Tdata[10].data = "jskjdfslkdfjlskrereliabletensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjfdlskfjlskjhanmyothisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmeinwnmeanderingexperienceIwilldispense20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[10].size = 1024;


	Tdata[11].data = "eaEnjoythepowerandbeautyofyouryonderingexperienceIwilldispensethisadvicenowuthOhneveetensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownmnderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmeinrmindYouwillnoturereliabl20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[11].size = 1024;

	Tdata[12].data = "autyjfdlskfjlskjhanmyownofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskmnderstandeandbeautyofyouryouthOhneverminanderingexperienceIwilldispensethisadvicenowEnjoythepowerdYouwillnoturereliabletensethisadvicenowEnjoythepowerandbethepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldisp";	
	Tdata[12].size = 1024;

	Tdata[13].data = "litylaybeforeyojdfslkdfjlskjfdlskfjlskjhanmyowuandhowfabulousyoureallylookedYouareienceIwilldispautyofyoldkjslkfjskjfljkjlkjsknmnderstandeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnoturereliabletensethisadvicenowEnjoythepowerandbethepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibi";	
	Tdata[13].size = 1024;

	Tdata[14].data = "spenldindbeautyofyouryoutsethisadvicenowEnjoythepowerarereliablethanmyownmeanderingexperienceIwilhOhnutyo0yeaevermindYouwillnotunderstandtfyouryouthuntiltheyvefadedButtrustmein2hepowerandbearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouareienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskj";	
	Tdata[14].size = 1024;


	Tdata[15].data = "meanderulousyoureallylookedYouareienceIwilldispingexperienceIwilldispensrereliabletensethisadvicenowEnjoythepowerandbeautyofyoldkjslkfjskjfljkjlkjskjdfslkdfjlskjfdlskfjlskjhanmyownethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfabulousyoureallylookedYouarenotasfatasyouimagineDontworryaboutthefutureOrworrybutknowthatKurtVonnegutsCommencementAddressatMITLadiesandgentlemenoftheclassof97WearsunscreenIfIcouldofferyouonlyonetipforthefuturesunscreenwouldbeitThelongtermbenefitsofsunscreenhavebeenprovedbyscientistswhereastherestofmyadvicehasnobasismorereliablethanmyownmeanderingexperienceIwilldispensethisadvicenowEnjoythepowerandbeautyofyouryouthOhnevermindYouwillnotunderstandthepowerandbeautyofyouryouthuntiltheyvefadedButtrustmein20yearsyoulllookbackatphotosofyourselfandrecallinawayyoucantgraspnowhowmuchpossibilitylaybeforeyouandhowfab";	
	Tdata[15].size = 1024;




}


void compute_digest(int index, int n){
	
	   SHA_INFO sha_info;
    
	   /*pthread_mutex_lock(&mutex_print);       
	   printf("Sha: Worker %d started.\n",n);
	   fprintf(fileout_sha,"Sha: Worker %d started.\n",n);
	   pthread_mutex_unlock(&mutex_print);*/

	   sha_stream(&sha_info, Tdata[index].data,Tdata[index].size);
           
	   /*pthread_mutex_lock(&mutex_print);    
	   printf("Sha: Worker %d finished.\n",n);
	   fprintf(fileout_sha,"\nSha: Worker %d finished.\n",n);
	   pthread_mutex_unlock(&mutex_print);
		*/

	   //sha_print_to_file(&sha_info,fileout_sha);	

	   pthread_mutex_lock(&mutex_print);

           fprintf(fileout_sha,"%08lx %08lx %08lx %08lx %08lx\n",
	   sha_info.digest[0], sha_info.digest[1], sha_info.digest[2],
   	   sha_info.digest[3], sha_info.digest[4]);

	   pthread_mutex_unlock(&mutex_print);
 
}

void *parallel_SHA_Dig_Calc(void *params){
	
       int index;
       parameters *param=(parameters *)params;
       
       
      
       for (index=param->s_index; index<param->e_index; index++){
		compute_digest(index,param->no);
       }
        
}

void init_workers(){
	
	pthread_attr_init(&sha_attr);
	pthread_mutex_init(&string_mutex,NULL);
	//pthread_attr_setdetachstate(&sha_attr,2); // 2 indicates that software is sha	
}

void createWorkers(){
	long index;
	for ( index= 0 ; index<no_workers; index++)
        {
		    pthread_create(&workers[index],&sha_attr,(void *)parallel_SHA_Dig_Calc,(void *)&paramsArr[index]);
	}

	pthread_executeThread();
	
}
void waitForWorkersToFinish(){
	int index;
	    for ( index= 0 ; index<no_workers; index++)
            {
	    	   pthread_join(workers[index],NULL);
		   
	    }
	    	   
 }                                               
 
void divide_into_sub_tasks(){

	  int i;
	  int s_index=0;
	  
	
	  for (i=0;i<no_workers;i++) {
			   paramsArr[i].s_index=s_index;
			   paramsArr[i].e_index=s_index+partition_size;
			   paramsArr[i].name="Worker";
			   paramsArr[i].no=i+1;
			   s_index+=partition_size;
			      
	  }
	
	  // It handles even not of jobs and odd no of workers vice versa.
	   paramsArr[i-1].e_index=paramsArr[i-1].e_index+(no_files%no_workers);
   	  
}

void show_sub_tasks() {

		int i;
			for (i=0;i<no_workers;i++) {
				   
				   pthread_mutex_lock(&mutex_print);
				   fprintf(fileout_sha,"%d   ",paramsArr[i].s_index);
				   fprintf(fileout_sha,"%d   ",paramsArr[i].e_index);
				   fprintf(fileout_sha,"%s ",paramsArr[i].name);
				   fprintf(fileout_sha,"%d\n ",paramsArr[i].no);
				   pthread_mutex_unlock(&mutex_print);
		  }
}

void init(){
	  partition_size= no_files/no_workers; //MAX_NO_FILES/no_workers;
	  divide_into_sub_tasks();
	  if (PRINT)
		show_sub_tasks();
}   

void parallel_process(){
	 	       
	    init();
	    init_workers();
	    createWorkers();
	    waitForWorkersToFinish();
}

void sequential_process(){
	
	      int index;
       
       printf("*******  Digest Calucation:Sequential Process  *******\n");
       
       for (index=0; index<no_files; index++){
       	if (PRINT){
       	    printf("-------*******************************-------------------\n");
       	    printf("Computing Digest for file");
       	  }
       	    
       	       compute_digest(index,0);
       	   
     
      }
	
}


int main0_sha(int argc,char *argv[])
{

		


	char *token;
      if (argc<2 ||argc>3){
      	  printf("|-----------------------------------------------------------------------|\n");
      	  printf("	Error: Insufficient Parameters.                             \n");
      	  printf("	Commands to run!                             \n");
      	  printf("	Command Format: OjbectFileName -Sequential(S)/Parallel(P) -Workers!\n");
      	  printf("	Example: sequential SHA: ' ./sha -S   '!                             \n");
      	  printf("	Example: parallel SHA  : ' ./sha -P -2 '!\n");
      	  printf("|---------------------------------------------------------------------- |\n");
      	  exit(0);
      }else {
      	    
      	    token=argv[1];
      	    if (*token=='-') 
            {
      	       token++;
      	       if (strcmp(token,"S")==0){
      	      	    readFilesData();
	            sequential_process();
	       }else if (strcmp(token, "P")==0){
		    token= argv[2];
		    token++;
		    readFilesData();


	
			
		    parallel_process();	
			      	              
		}
               else {
			 		  printf("ERROR: Unknown Parameters!\n");
			      	  	}//end-if	  	
      	  }else {
      	  	 printf("ERROR: Unknown Parameters!\n");
      	  }
      	  	
      }

	
    return(0);
}


