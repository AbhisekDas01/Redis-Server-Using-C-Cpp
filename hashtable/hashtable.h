#pragma once //Only include this file once during the compilation of a single source file, no matter how many times it gets requested.


#include <cstdint> //used for uint64_t
#include <cstddef> //for NULL && size_t

//intrusive list node. An intrusive hashtable doesn’t care about the data, but it still needs the hash value for the insertion.
struct HNode {
    HNode *next = NULL;
    uint64_t hcode = 0; //it will hold the hash value of each node
};

//Define the fixed-size hashtable
struct HTable {
    HNode **tab = NULL; //array of the buckets used in the hash table
    size_t mask = 0; //the size of the arrya should be multiple of 2 , mask = 2^n-1
    size_t size = 0; //number of keys stored currently
};