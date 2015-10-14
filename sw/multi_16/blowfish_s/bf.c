#include <stdio.h>
#include "blowfish.h"

#ifndef acPthread_H_
#include "../../acPthread.h"
#endif
extern FILE *fileout_blowfishenc;
extern FILE *filein_blowfishenc;
extern FILE *fileout_blowfishdec;
extern FILE *filein_blowfishdec;

extern pthread_mutex_t mutex_print;


void 
main_blowfish(char *op)
{
	BF_KEY key;
	unsigned char ukey[8];
	unsigned char indata[40],outdata[40],ivec[8];
	int num;
	int by=0,i=0;
	int encordec=-1;
	char *cp,ch;
	FILE *fp,*fp2;

int argc = 5;
char *argv[5];

argv[0]="";
argv[1]=op;
argv[2]="";
argv[3]="";
argv[4]="1234567890abcdeffedcba0987654321";

if (argc<3)
{
	printf("Usage: blowfish {e|d} <intput> <output> key\n");
	exit(-1);
}

if (*argv[1]=='e' || *argv[1]=='E')
{
	encordec = 1;
	fp = filein_blowfishenc;
	fp2 = fileout_blowfishenc;
}
else if (*argv[1]=='d' || *argv[1]=='D')
{

	encordec = 0;
	fp = filein_blowfishdec;
	fp2 = fileout_blowfishdec;
}
else
{
	printf("Usage: blowfish {e|d} <intput> <output> key\n");
	exit(-1);
}

/* Read the key */
cp = argv[4];

/*pthread_mutex_lock(&mutex_print);
printf("\nblowfish opcao %s encordec %d fp %x fp2 %x  \n",op, encordec, fp, fp2);
printf("\nchave %s \n",cp);
pthread_mutex_unlock(&mutex_print);*/

while(i < 64 && *cp)    /* the maximum key length is 32 bytes and   */
{                       /* hence at most 64 hexadecimal digits      */
	ch = toupper(*cp++);            /* process a hexadecimal digit  */
	if(ch >= '0' && ch <= '9')
		by = (by << 4) + ch - '0';
	else if(ch >= 'A' && ch <= 'F')
		by = (by << 4) + ch - 'A' + 10;
	else                            /* error if not hexadecimal     */
	{
		printf("key must be in hexadecimal notation\n");
		exit(-1);
	}

	/* store a key byte for each pair of hexadecimal digits         */
	if(i++ & 1)
		ukey[i / 2 - 1] = by & 0xff;
}

BF_set_key(&key,8,ukey);

if(*cp)
{
	printf("Bad key value.\n");
	exit(-1);
}

/* open the input and output files */
/*if ((fp = fopen(argv[2],"r"))==0)
{
	printf("Usage: blowfish {e|d} <intput> <output> key\n");
	exit(-1);
};
if ((fp2 = fopen(argv[3],"w"))==0)
{
	printf("Usage: blowfish {e|d} <intput> <output> key\n");
	exit(-1);
};*/



i=0;
while(!feof(fp))
{
	int j;
	while(!feof(fp)&& i<40)
		indata[i++]=getc(fp);

	BF_cfb64_encrypt(indata,outdata,i,&key,ivec,&num,encordec);

        pthread_mutex_lock(&mutex_print);
	for(j=0;j<i;j++)
	{
		//printf("%c",outdata[j]);
		//fprintf(fp2,"%c",outdata[j]);
		fputc(outdata[j],fp2);
	}
	pthread_mutex_unlock(&mutex_print);
	i=0;
}

fclose(fp);
//close(fp2);

//exit(1);
return;
}



