/*******************************************************
                          cache.cc
                  Amro Awad & Yan Solihin
                           2013
                {ajawad,solihin}@ece.ncsu.edu
********************************************************/

#include <stdlib.h>
#include <assert.h>
#include "cache.h"
using namespace std;

MSI_Cache::MSI_Cache(int s, int a, int b) : Cache(s, a, b)
{
	inv2exc = inv2shd = mod2shd = exc2shd = shd2mod = inv2mod = exc2mod = own2mod = mod2own = shd2inv = c2c = interventions = invalidations = flushCount = 0;
}

/**you might add other parameters to Access()
since this function is an entry point 
to the memory hierarchy (i.e. caches)**/
void MSI_Cache::Access(ulong addr,uchar op)
{
	currentCycle++;/*per cache global counter to maintain LRU order 
			among cache ways, updated on every cache access*/
        	
	if(op == 'w') writes++;
	else          reads++;
	
	cacheLine * line = findLine(addr);
	if(line == NULL)/*miss*/
	{
		if(op == 'w') writeMisses++;
		else readMisses++;

		cacheLine *newline = fillLine(addr);
   		if(op == 'w') 
		{
			newline->setFlags(MODIFIED);
			inv2mod++;
			::postOnBus(procId, addr, BusRdX);
		}
		else
		{
			 newline->setFlags(SHARED);   
			 inv2shd++;
			 ::postOnBus(procId, addr, BusRd);
		}
		
	}
	else
	{
		/**since it's a hit, update LRU and update dirty flag**/
		updateLRU(line);
	//	if(op == 'w') line->setFlags(MODIFIED);
		
		if(line->getFlags() == SHARED)
		{
			if(op == 'w')
			{
				line->setFlags(MODIFIED);
				shd2mod++;
				::postOnBus(procId, addr, BusRdX);
			}
		}
	}
}

void MSI_Cache::printStats()
{
cout<<"01. number of reads:                   		"<<reads<<"\n";
cout<<"02. number of read misses:             		"<<readMisses<<"\n";
cout<<"03. number of writes:                  		"<<writes<<"\n";
cout<<"04. number of write misses:            		"<<writeMisses<<"\n";
cout<<"05. number of write backs:             		"<<writeBacks<<"\n";
cout<<"06. number of invalid to exclusive (INV->EXC):	"<<inv2exc<<"\n";
cout<<"07. number of invalid to shared (INV->SHD):	"<<inv2shd<<"\n";
cout<<"08. number of modified to shared (MOD->SHD):	"<<mod2shd<<"\n";
cout<<"09. number of exclusive to shared (EXC->SHD):	"<<exc2shd<<"\n";
cout<<"10. number of shared to modified (SHD->MOD):	"<<shd2mod<<"\n";
cout<<"11. number of invalid to modified (INV->MOD):	"<<inv2mod<<"\n";
cout<<"12. number of exclusive to modified (EXC->MOD):	"<<exc2mod<<"\n";
cout<<"13. number of owned to modified (OWN->MOD):	"<<own2mod<<"\n";
cout<<"14. number of modified to owned (MOD->OWN):	"<<mod2own<<"\n";
cout<<"15. number of shared to invalid (SHD->INV):	"<<shd2inv<<"\n";
cout<<"16. number of cache to cache transfers:    	"<<c2c<<"\n";
cout<<"17. number of interventions:               	"<<mod2shd<<"\n";
cout<<"18. number of invalidations:               	"<<shd2inv<<"\n";
cout<<"19. number of flushes:                     	"<<flushCount<<"\n";
}

void MSI_Cache::snoop(ulong addr, BusOps busOp)
{
   cacheLine *line = findLine(addr);

   if(line == NULL)
        return;

   switch(busOp)
   {
        case BusRd:
                if(line->getFlags() == MODIFIED)
                {
                        line->setFlags(SHARED);
                        mod2shd++;
			flushCount++;
                }
        break;
        case BusRdX:
                if(line->getFlags() == MODIFIED)
                {
                        line->setFlags(INVALID);
			flushCount++;
                }
		if(line->getFlags() == SHARED)
		{
			line->setFlags(INVALID);
			shd2inv++;
		}

        break;
	case Flush:
	break;
   }
}

