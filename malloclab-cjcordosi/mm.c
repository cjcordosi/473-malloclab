/*
 * mm.c
 *
 * Name: Cross Cordosi
 *
 * mm.c is designed to take in the size of a piece of data they wish to store,
 * and allocate space for it within the heap. It is also capable of freeing these
 * spaces. allowing for new data to be written into this area. Overwriting the old
 * data with new data is reallocation. This program searches through the heap for
 * available space, and if there is none, allocates new space for the users data.
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#include "mm.h"
#include "memlib.h"

/*
 * If you want to enable your debugging output and heap checker code,
 * uncomment the following line. Be sure not to have debugging enabled
 * in your final submission.
 */
// #define DEBUG

#ifdef DEBUG
/* When debugging is enabled, the underlying functions get called */
#define dbg_printf(...) printf(__VA_ARGS__)
#define dbg_assert(...) assert(__VA_ARGS__)
#else
/* When debugging is disabled, no code gets generated */
#define dbg_printf(...)
#define dbg_assert(...)
#endif /* DEBUG */

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#define memset mem_memset
#define memcpy mem_memcpy
#endif /* DRIVER */

/* What is the correct alignment? */
#define ALIGNMENT 16

typedef struct header_t {
    size_t size;
    struct header_t* next;
    struct header_t* prev;
    int free; // 0 is free, 1 is allocated
} header_t;

/* rounds up to the nearest multiple of ALIGNMENT */
static size_t align(size_t x)
{
    return ALIGNMENT * ((x+ALIGNMENT-1)/ALIGNMENT);
}

struct header_t* prologue;
struct header_t* free_head;
struct header_t* free_last;

/*
 * Initialize: returns false on error, true on success.
 */
bool mm_init(void)
{
    // Create the prologue header, and set base values
    prologue = mem_sbrk(align(sizeof(struct header_t)));
    prologue->size = align(sizeof(struct header_t));
    prologue->next = prologue;
    prologue->prev = prologue;
    prologue->free = 1;

    // Initialize the free list first and last nodes to the prologue
    free_head = prologue;
    free_last = prologue;

    return true;
}

/*************** Helper Functions  *******************/

/*
 * find_fit
 */
void *find_fit(size_t asize){
    
    bool fitfound = false;
    struct header_t* current = free_head;

    // While we havent found a fit yet, and we are not at the last free node
    while(!fitfound && current != free_last){
        // If the size of the current node is greater than or equal to what we are looking for,
        // we have found a fit
        if(current->size >+ asize){
            fitfound = true;
        }
        current = current->next;
    }
    // If we found a fit, return that block
    if(fitfound){
        return current;
    }else{
        return NULL;
    }
}

/*************** Helper Functions  *******************/

/*
 * malloc
 */
void* malloc(size_t size)
{
    // If we are trying to malloc no size, end early
    if(size == 0){
        return NULL;
    }

    // create a size variable equal to the alligned size of a header + the desired size
    size_t asize = align(align(sizeof(struct header_t)) + size);

    // allocate the size in the heap and return a pointer to the beginning of the data (past the header)
    struct header_t* ptr = mem_sbrk(asize);
    return (char*)ptr + (align(sizeof(struct header_t)));


    // This code is meant to search the free list for the first fitting free block,
    // however does not currently work
    // struct header_t* freespace = find_fit(asize);
    // if(freespace != NULL){
    //     freespace->prev->next = freespace->next;
    //     freespace->next->prev = freespace->prev; 
    //     return (char*)freespace + (align(sizeof(struct header_t)));
    // }else{
    //     struct header_t* ptr = mem_sbrk(asize);
    //     return (char*)ptr + (align(sizeof(struct header_t)));
    // }
    // return NULL;
}

/*
 * free
 */
void free(void* ptr)
{
    if(ptr == NULL){
        return;
    }else{
        // create a free block header and set its free bit to 0
        struct header_t* freed_block = (ptr - sizeof(struct header_t));
        freed_block->free = 0;
        
        // This block of pointer code deals with handling the explicit free list,
        // adding nodes and moving around pointers to ensure circlular continuity
        freed_block->next = free_head;
        freed_block->prev = free_last;
        free_head->prev = freed_block;
        free_last->next = freed_block;
        free_head = freed_block;
    }
    return;
}

/*
 * realloc
 */
void* realloc(void* oldptr, size_t size)
{
    // If the pointer passed in is null, malloc with size,
    // and if size = 0, free with the pointer, otherwise,
    // malloc with size, and use memcpy() with the returned address
    // to overwrite the data (reallocate it)
    if(oldptr == NULL){
        malloc(size);
    }else if(size == 0){
        free(oldptr);
    }else{
        void* ptr = malloc(size);
        memcpy(ptr, oldptr, size);
        return ptr;
    }

    return NULL;
}

/*
 * calloc
 * This function is not tested by mdriver, and has been implemented for you.
 */
void* calloc(size_t nmemb, size_t size)
{
    void* ptr;
    size *= nmemb;
    ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

/*
 * Returns whether the pointer is in the heap.
 * May be useful for debugging.
 */
static bool in_heap(const void* p)
{
    return p <= mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Returns whether the pointer is aligned.
 * May be useful for debugging.
 */
static bool aligned(const void* p)
{
    size_t ip = (size_t) p;
    return align(ip) == ip;
}

/*
 * mm_checkheap
 */
bool mm_checkheap(int lineno)
{
#ifdef DEBUG
    /* Write code to check heap invariants here */
    /* IMPLEMENT THIS */
#endif /* DEBUG */
    return true;
}