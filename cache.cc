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

Cache::Cache(int s,int a,int b )
{
   ulong i, j;
   reads = readMisses = writes = 0; 
   writeMisses = writeBacks = currentCycle = 0;
   inv2exc = inv2shd = mod2shd = exc2shd = shd2mod = inv2mod = exc2mod = own2mod = mod2own = shd2inv = c2c = interventions = invalidations = flushCount = 0;

   size       = (ulong)(s);
   lineSize   = (ulong)(b);
   assoc      = (ulong)(a);   
   sets       = (ulong)((s/b)/a);
   numLines   = (ulong)(s/b);
   log2Sets   = (ulong)(log2(sets));   
   log2Blk    = (ulong)(log2(b));   
  
   //*******************//
   //initialize your counters here//
   //*******************//
 
   tagMask =0;
   for(i=0;i<log2Sets;i++)
   {
		tagMask <<= 1;
        tagMask |= 1;
   }
   
   /**create a two dimentional cache, sized as cache[sets][assoc]**/ 
   cache = new cacheLine*[sets];
   for(i=0; i<sets; i++)
   {
      cache[i] = new cacheLine[assoc];
      for(j=0; j<assoc; j++) 
      {
	   cache[i][j].invalidate();
      }
   }      

   procId = numCaches++;   
}

int Cache::numCaches = 0;

/**you might add other parameters to Access()
since this function is an entry point 
to the memory hierarchy (i.e. caches)**/
void Cache::Access(ulong addr,uchar op)
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
   		if(op == 'w') newline->setFlags(MODIFIED);    
		
	}
	else
	{
		/**since it's a hit, update LRU and update dirty flag**/
		updateLRU(line);
		if(op == 'w') line->setFlags(MODIFIED);
	}
}

/*look up line*/
cacheLine * Cache::findLine(ulong addr)
{
   ulong i, j, tag, pos;
   
   pos = assoc;
   tag = calcTag(addr);
   i   = calcIndex(addr);
  
   for(j=0; j<assoc; j++)
	if(cache[i][j].isValid())
	        if(cache[i][j].getTag() == tag)
		{
		     pos = j; break; 
		}
   if(pos == assoc)
	return NULL;
   else
	return &(cache[i][pos]); 
}

/*upgrade LRU line to be MRU line*/
void Cache::updateLRU(cacheLine *line)
{
  line->setSeq(currentCycle);  
}

/*return an invalid line as LRU, if any, otherwise return LRU line*/
cacheLine * Cache::getLRU(ulong addr)
{
   ulong i, j, victim, min;

   victim = assoc;
   min    = currentCycle;
   i      = calcIndex(addr);
   
   for(j=0;j<assoc;j++)
   {
      if(cache[i][j].isValid() == 0) return &(cache[i][j]);     
   }   
   for(j=0;j<assoc;j++)
   {
	 if(cache[i][j].getSeq() <= min) { victim = j; min = cache[i][j].getSeq();}
   } 
   assert(victim != assoc);
   
   return &(cache[i][victim]);
}

/*find a victim, move it to MRU position*/
cacheLine *Cache::findLineToReplace(ulong addr)
{
   cacheLine * victim = getLRU(addr);
   updateLRU(victim);
  
   return (victim);
}

/*allocate a new line*/
cacheLine *Cache::fillLine(ulong addr)
{ 
   ulong tag;
  
   cacheLine *victim = findLineToReplace(addr);
   assert(victim != 0);
   if(victim->getFlags() == MODIFIED) writeBack(addr);
   	
   tag = calcTag(addr);   
   victim->setTag(tag);
   victim->setFlags(SHARED);    
   /**note that this cache line has been already 
      upgraded to MRU in the previous function (findLineToReplace)**/

   return victim;
}

void Cache::printStats()
{
cout<<"01. number of reads:                             "<<reads<<"\n";
cout<<"02. number of read misses:                       "<<readMisses<<"\n";
cout<<"03. number of writes:                            "<<writes<<"\n";
cout<<"04. number of write misses:                      "<<writeMisses<<"\n";
cout<<"05. number of write backs:                       "<<writeBacks<<"\n";
cout<<"06. number of invalid to exclusive (INV->EXC):   "<<inv2exc<<"\n";
cout<<"07. number of invalid to shared (INV->SHD):      "<<inv2shd<<"\n";
cout<<"08. number of modified to shared (MOD->SHD):     "<<mod2shd<<"\n";
cout<<"09. number of exclusive to shared (EXC->SHD):    "<<exc2shd<<"\n";
cout<<"10. number of shared to modified (SHD->MOD):     "<<shd2mod<<"\n";
cout<<"11. number of invalid to modified (INV->MOD):    "<<inv2mod<<"\n";
cout<<"12. number of exclusive to modified (EXC->MOD):  "<<exc2mod<<"\n";
cout<<"13. number of owned to modified (OWN->MOD):      "<<own2mod<<"\n";
cout<<"14. number of modified to owned (MOD->OWN):      "<<mod2own<<"\n";
cout<<"15. number of shared to invalid (SHD->INV):      "<<shd2inv<<"\n";
cout<<"16. number of cache to cache transfers:          "<<c2c<<"\n";
cout<<"17. number of interventions:                     "<<interventions<<"\n";
cout<<"18. number of invalidations:                     "<<invalidations<<"\n";
cout<<"19. number of flushes:                           "<<flushCount<<"\n";
}

int Cache::snoop(ulong addr, BusOps busOp)
{
	return -1;  
}
