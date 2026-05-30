#include <cassert>
#include <cstdlib>
#include "hashtable.h"

//initialize a fixed size hash table 
static void hashTableInit(HTable *htab , size_t n) {
    assert(n > 0 && (n & (n-1)) == 0); //check if the size is > 0 && n must be a power of 2 (for faster module operation)
    htab->tab = (HNode **)calloc(n , sizeof(HNode *));  //allocate the n size block of memeory 
    htab->mask = n-1;
    htab->size = 0;
}

