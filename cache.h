/*******************************************************
                          cache.h
              Amro Awad & Yan Solihin
                           2013
                {ajawad,solihin}@ece.ncsu.edu
********************************************************/

#ifndef CACHE_H
#define CACHE_H

#include <cmath>
#include <iostream>
#include<stdio.h>

typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int uint;

enum BusOps
{
	BusRd = 0,
	BusRdX,
	Flush
};

void postOnBus(int senderId, ulong addr, BusOps busOp);

/****add new states, based on the protocol****/
enum{
	INVALID = 0,
	MODIFIED,
	SHARED,
	EXCLUSIVE
};

class cacheLine 
{
protected:
   ulong tag;
   ulong Flags;   // 0:invalid, 1:valid, 2:dirty 
   ulong seq; 
 
public:
   cacheLine()            { tag = 0; Flags = 0; }
   ulong getTag()         { return tag; }
   ulong getFlags()			{ return Flags;}
   ulong getSeq()         { return seq; }
   void setSeq(ulong Seq)			{ seq = Seq;}
   void setFlags(ulong flags)			{  Flags = flags;}
   void setTag(ulong a)   { tag = a; }
   void invalidate()      { tag = 0; Flags = INVALID; }//useful function
   bool isValid()         { return ((Flags) != INVALID); }
};

class Cache
{
protected:
   ulong size, lineSize, assoc, sets, log2Sets, log2Blk, tagMask, numLines;
   ulong reads,readMisses,writes,writeMisses,writeBacks;
   int procId;

   //******///
   //add coherence counters here///
   //******///

   cacheLine **cache;
   ulong calcTag(ulong addr)     { return (addr >> (log2Blk) );}
   ulong calcIndex(ulong addr)  { return ((addr >> log2Blk) & tagMask);}
   ulong calcAddr4Tag(ulong tag)   { return (tag << (log2Blk));}

   static int numCaches;   
public:
    ulong currentCycle;  

    Cache(int,int,int);
   ~Cache() { delete cache;}
   
   cacheLine *findLineToReplace(ulong addr);
   cacheLine *fillLine(ulong addr);
   cacheLine * findLine(ulong addr);
   cacheLine * getLRU(ulong);
   
   ulong getRM(){return readMisses;} ulong getWM(){return writeMisses;} 
   ulong getReads(){return reads;}ulong getWrites(){return writes;}
   ulong getWB(){return writeBacks;}

   int getProcId() { return procId; }   
   void writeBack(ulong)   {writeBacks++;}
   void Access(ulong,uchar);
   void printStats();
   void updateLRU(cacheLine *);

   static int getNumCaches() { return numCaches; }

   void snoop(ulong addr, BusOps busOp);

   //******///
   //add other functions to handle bus transactions///
   //******///

};

class MSI_Cache: public Cache
{
 public:
	ulong inv2exc, inv2shd, mod2shd, exc2shd, shd2mod, inv2mod, exc2mod, own2mod, mod2own, shd2inv;
	ulong c2c, interventions, invalidations;
	ulong flushCount;

	MSI_Cache(int,int,int);

	void Access(ulong, uchar);
	void printStats();
	void snoop(ulong addr, BusOps busOp);
};
#endif
