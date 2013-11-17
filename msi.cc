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
	//inv2exc = inv2shd = mod2shd = exc2shd = shd2mod = inv2mod = exc2mod = own2mod = mod2own = shd2inv = c2c = interventions = invalidations = flushCount = 0;
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

int MSI_Cache::snoop(ulong addr, BusOps busOp)
{
   cacheLine *line = findLine(addr);
   
   if(line == NULL)
        return 0;

   switch(busOp)
   {
        case BusRd:
                if(line->getFlags() == MODIFIED)
                {
                        line->setFlags(SHARED);
                        mod2shd++;
			flushCount++;
			interventions++;
			::flush(addr);
                }
        break;
        case BusRdX:
                if(line->getFlags() == MODIFIED)
                {
                        line->setFlags(INVALID);
			flushCount++;
			::flush(addr);
			invalidations++;
                }
		if(line->getFlags() == SHARED)
		{
			line->setFlags(INVALID);
			shd2inv++;
			invalidations++;
		}

        break;
	case Flush:
		c2c++;
	break;
	case BusUpgr:
	break;
   }

   return 0;
}

