/**
 * @file hashring.h
 * @author Fruerlund
 * @brief 
 * @version 0.1
 * @date 2024-08-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HASHRING_H
#define HASHRING_H

#include "common-defines.h"



/* 
[**************************************************************************************************************************************************]
                                                            STRUCTUES AND PROTOTYPES
[**************************************************************************************************************************************************]
*/

#define ELEMENT_SERVER  0x1
#define ELEMENT_EMPTY   0x2
#define ELEMENT_KEY     0x3

#define OP_NODEINSERT   0x4
#define OP_NODEDEL      0x5

/**
 * @brief Describes a element in the hash ring that holds a key, value pair.
 */
typedef struct ring_element_key_t {

    char *key;                  
    char *store;
    
} ring_element_key_t;


/**
 * @brief Describes a element in the hash ring that represents a data store.
 */
typedef struct ring_element_server_t {

    char *ip;                           /* IP */
    int port;                           /* PORT */
    size_t size;                        /* Number of key, value pairs in hash store.  */
    size_t maxsize;                     /* Maximum number of kvp in a hash store. */
    uint32_t rangestart;                /* Start of key ranges */
    uint32_t rangeend;                  /* End of key ranges */
    uint32_t numberofvirtualnodes;      /* Number of virtual nodes */
    bool isvirtualnode;                 /* Virtual node indicator */

} ring_element_server_t;

/**
 * @brief Describes a empty element in the hash ring.
 */
typedef struct ring_element_empty_t {

} ring_element_empty_t;



/**
 * @brief Describes a chunk of the hash ring that either represents a empty spot, data store or key, value, pair
 */
typedef struct ring_element_t {

    uint32_t hash;
    struct ring_element_t *left;            /* Pointer to left slice */
    struct ring_element_t *right;           /* Pointer to right slice*/
    struct ring_element_t *parent;
    uint8_t type;                           /* Used to indicate the type(empty, data or server) that his slice holds */

    union element {
        ring_element_server_t *server;
        ring_element_key_t *data;
        ring_element_empty_t *empty;
    } element;

} ring_element_t;


/**
 * @brief Describes a hash ring using consistent hashing.
 */

typedef uint32_t (*hashring_hash_t)(char *key);

typedef struct hashring_t {

    size_t size;                        /*  Size of the hash ring            */
    size_t count;                       /*  Current elemenets in hash ring   */
    struct ring_element_t **elements;
    struct ring_element_t *tree;        /*  Binary search tree representation of hash values of servers */
    uint32_t *servers;                  /*  Pointer to an array of hash values of servers */
    size_t numberofservers;             /*  Number of servers in hash ring */
    hashring_hash_t fn;                 /*  Pointer to the method responsible for hashing   */

} hashring_t;



/* 
[**************************************************************************************************************************************************]
                                                            METHODS / FUNCTIONS
[**************************************************************************************************************************************************]
*/
uint32_t hashring_deleteelement(ring_element_t *e);
ring_element_t * hashring_addserver(hashring_t *r, char *ip, uint32_t virtualNodes);
uint32_t hashring_removeserver(hashring_t *r, char *ip);
void hashring_destroy(hashring_t *r);
hashring_t * hashring_create(size_t size, hashring_hash_t fn);
ring_element_t * hashring_lookupkey(hashring_t *r, char *key );
uint32_t hashring_generatenodesarray(hashring_t *r);
uint32_t hashring_serverfindrange(hashring_t *r, uint32_t start);
uint32_t hashring_addvirtualnodes(hashring_t *r, char *ip, uint32_t i);
uint32_t hashring_updateranges(hashring_t *r);
uint32_t hashring_remapkeys_add(hashring_t *r, ring_element_t *e);
uint32_t hashring_remapkeys_del(hashring_t *r, ring_element_t *e);
ring_element_t *hashring_lookupserver(hashring_t *r, char *ip);
uint32_t hashring_deleteelement(ring_element_t *e);
bool hashring_objectexists(hashring_t *r, uint32_t index, uint8_t type);
uint32_t hashring_addstore_iterative(hashring_t *r, ring_element_t *e);
uint32_t hashring_addstore_bst(hashring_t *r, ring_element_t *e);
uint32_t hashring_addkey(hashring_t *r, char *key);
uint32_t hashring_removekey(hashring_t *r, char *key);
void hashring_showranges(hashring_t *r);


#endif