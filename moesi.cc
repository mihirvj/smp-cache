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

MOESI_Cache::MOESI_Cache(int s, int a, int b) : Cache(s, a, b)
{
}

/**you might add other parameters to Access()
since this function is an entry point 
to the memory hierarchy (i.e. caches)**/
void MOESI_Cache::Access(ulong addr,uchar op)
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

		if(line->getFlags() == OWNED)
		{
			if(op == 'w')
			{
				line->setFlags(MODIFIED);
				own2mod++;
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

int MOESI_Cache::snoop(ulong addr, BusOps busOp)
{
   cacheLine *line = findLine(addr);
   
   if(line == NULL)
        return 0;

   int flushed = 0;

   switch(busOp)
   {
        case BusRd:
                if(line->getFlags() == MODIFIED)
                {
                        line->setFlags(OWNED);
                        mod2own++;
			//flushCount++; //mark
			//::flush(addr); 
			//flushed = 1;
                }
		if(line->getFlags() == EXCLUSIVE)
		{
			line->setFlags(SHARED);
			exc2shd++;
			interventions++;
			::flush(addr); // optional
			flushed = 1;
		}
		if(line->getFlags() == SHARED)
		{
			// optional flush
		}

		if(line->getFlags() == OWNED)
		{
			flushCount++; 
			::flush(addr);
			flushed = 1;
		}
        break;
        case BusRdX:
                if(line->getFlags() == MODIFIED)
                {
                        line->setFlags(INVALID);
			flushCount++; 
			::flush(addr);
			flushed = 1;
			invalidations++;
                }
		if(line->getFlags() == SHARED)
		{
			line->setFlags(INVALID);
			shd2inv++;
			invalidations++;
		}
		if(line->getFlags() == EXCLUSIVE)
		{
			line->setFlags(INVALID);
			invalidations++;
		}

		if(line->getFlags() == OWNED)
		{
			line->setFlags(INVALID);
			flushCount++;
			invalidations++;
			::flush(addr);
			flushed = 1;
		}
        break;
	case Flush:
		c2c++;
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
			//flushCount++;
			::flush(addr); 
			flushed = 1;
			invalidations++;
		}
		if(line->getFlags() == OWNED)
		{
			line->setFlags(INVALID);
			// dont flush
			invalidations++;
		}
	break;
   }

   return flushed;
}
