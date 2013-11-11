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

MESI_Cache::MESI_Cache(int s, int a, int b) : Cache(s, a, b)
{

}

/**you might add other parameters to Access()
since this function is an entry point 
to the memory hierarchy (i.e. caches)**/
void MESI_Cache::Access(ulong addr,uchar op)
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
			if(!copiesExist(procId, addr))
			{
				newline->setFlags(EXCLUSIVE);
				inv2exc++;
			}
			else
			{
				newline->setFlags(SHARED);   
			 	inv2shd++;
				c2c++;
				//interventions++;
			}

			::postOnBus(procId, addr, BusRd);
		}
		
	}
	else
	{
		/**since it's a hit, update LRU and update dirty flag**/
		updateLRU(line);
		
		if(line->getFlags() == SHARED)
		{
			if(op == 'w')
			{
				line->setFlags(MODIFIED);
				shd2mod++;
				::postOnBus(procId, addr, BusUpgr);
			}
		}
		if(line->getFlags() == EXCLUSIVE)
		{
			if(op == 'w')
			{
				line->setFlags(MODIFIED);
				exc2mod++;
				// dont post anything
			}
		}
	}
}

void MESI_Cache::snoop(ulong addr, BusOps busOp)
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
			interventions++;
                }
		if(line->getFlags() == EXCLUSIVE)
		{
			line->setFlags(SHARED);
			exc2shd++;
			interventions++;
			// optional flush
		}
		if(line->getFlags() == SHARED)
		{
			// optional flush
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
			invalidations++;
			// optional flush
		}
		if(line->getFlags() == EXCLUSIVE)
		{
			line->setFlags(INVALID);
			// optional flush
		}

        break;
	case Flush:
	break;
	case BusUpgr:
		if(line->getFlags() == SHARED)
		{
			line->setFlags(INVALID);
			invalidations++;
			shd2inv++;
			// dont flush optional
		}
		if(line->getFlags() == MODIFIED)
		{
			line->setFlags(INVALID);
			flushCount++;
		}
	break;
   }
}

