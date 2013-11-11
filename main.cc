/*******************************************************
                          main.cc
                  Amro Awad & Yan Solihin
                           2013
                {ajawad,solihin}@ece.ncsu.edu
********************************************************/

#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include<string.h>

using namespace std;

#include "cache.h"

MSI_Cache **caches;

int main(int argc, char *argv[])
{
	
	ifstream fin;
	FILE * pFile;

	char protocols[3][6] = {"MSI", "MESI", "MOESI"};

	if(argv[1] == NULL){
		 printf("input format: ");
		 printf("./smp_cache <cache_size> <assoc> <block_size> <num_processors> <protocol> <trace_file> \n");
		 exit(0);
        }

	/*****uncomment the next five lines*****/
	int cache_size = atoi(argv[1]);
	int cache_assoc= atoi(argv[2]);
	int blk_size   = atoi(argv[3]);
	int num_processors = atoi(argv[4]);/*1, 2, 4, 8*/
	int protocol   = atoi(argv[5]);	 /*0:MSI, 1:MESI, 2:MOESI*/
	char *fname =  (char *)malloc(20);
 	fname = argv[6];

        caches = new MSI_Cache*[num_processors];

	for(int i=0 ;i< num_processors;i++)
	{
		caches[i] = new MSI_Cache(cache_size, cache_assoc, blk_size);
	}

	//****************************************************//
	printf("===== 506 SMP Simulator configuration =====\n");
	cout<<"L1_SIZE:         "<<cache_size<<endl;
	cout<<"L1_ASSOC:        "<<cache_assoc<<endl;
	cout<<"L1_BLOCKSIZE:    "<<blk_size<<endl;
	cout<<"NUMBER OF PROCESSORS:"<<num_processors<<endl;
	cout<<"COHERENCE PROTOCOL:  "<<protocols[protocol]<<endl;
	cout<<"TRACE FILE:          "<<fname<<endl;
 
	//*********************************************//
        //*****create an array of caches here**********//
	//*********************************************//	

	pFile = fopen (fname,"r");
	if(pFile == 0)
	{   
		printf("Trace file problem\n");
		exit(0);
	}

	char * line = NULL;
        size_t len = 0;
        ssize_t read;
	char *toks = NULL;
	
       pFile = fopen(fname, "r");
       if (pFile == NULL)
           exit(EXIT_FAILURE);

       while ((read = getline(&line, &len, pFile)) != -1) {
           toks = strtok(line, " ");

	   uchar i_proc = atoi(toks);

	   toks = strtok(NULL, " ");

	   uchar op = toks[0];

	   toks = strtok(NULL, " ");

	   ulong addr;

	   sscanf(toks, "%lx", &addr);

//	   printf("%d %c %lx\n", i_proc, op, addr);

	   caches[i_proc]->Access(addr, op);
       }

       if (line)
           free(line);

	fclose(pFile);

	//********************************//
	//print out all caches' statistics //
	//********************************//
	
	for(int i=0; i< num_processors;i++)
	{
		cout<<"===== Simulation results (Cache_"<<i<<")	=====\n";
		caches[i]->printStats();
	}
}

void postOnBus(int senderId, ulong addr, BusOps busOp)
{
	int numCaches = Cache::getNumCaches();

	for(int i=0; i < numCaches; i++)
	{
		if(i == senderId)
			continue;

		caches[i]->snoop(addr, busOp);
	}
}
