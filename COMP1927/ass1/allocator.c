//
//  COMP1927 Assignment 1 - Vlad: the memory allocator
//  allocator.c ... implementation
//
//  Created by Liam O'Connor on 18/07/12.
//  Modified by John Shepherd in August 2014, August 2015
//  Copyright (c) 2012-2015 UNSW. All rights reserved.
//

#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FREE_HEADER_SIZE  sizeof(struct free_list_header)
#define ALLOC_HEADER_SIZE sizeof(struct alloc_block_header)
#define MAGIC_FREE     0xDEADBEEF
#define MAGIC_ALLOC    0xBEEFDEAD

#define BEST_FIT       1
#define WORST_FIT      2
#define RANDOM_FIT     3

typedef unsigned char byte;
typedef u_int32_t vsize_t;
typedef u_int32_t vlink_t;
typedef u_int32_t vaddr_t;

typedef struct free_list_header {
   u_int32_t magic;  // ought to contain MAGIC_FREE
   vsize_t size;     // # bytes in this block (including header)
   vlink_t next;     // memory[] index of next free block
   vlink_t prev;     // memory[] index of previous free block
} free_header_t;

typedef struct alloc_block_header {
   u_int32_t magic;  // ought to contain MAGIC_ALLOC
   vsize_t size;     // # bytes in this block (including header)
} alloc_header_t;


// Global data

static byte *memory = NULL;   // pointer to start of allocator memory
static vaddr_t free_list_ptr; // index in memory[] of first block in free list
static vsize_t memory_size;   // number of bytes malloc'd in memory[]
static u_int32_t strategy;    // allocation strategy (by default BEST_FIT)
static vsize_t memory_total;
// Private functions

static void vlad_merge();


// Input: size - number of bytes to make available to the allocator
// Output: none
// Precondition: Size >= 1024
// Postcondition: `size` bytes are now available to the allocator
//
// (If the allocator is already initialised, this function does nothing,
//  even if it was initialised with different size)

void vlad_init(u_int32_t size)
{
    // TODO for Milestone 1

    //calculate size = 2^n
    if (size < 1024) size = 1024;
    vsize_t count = 1024;
    for (int i = 0; i<10; ++i){
    if (size > count)
        count *= 2;
    }
    size = count;

    memory = malloc(size);

    if (memory == NULL){
        fprintf(stderr, "vlad_init: Insufficient memory\n");
        exit(EXIT_FAILURE);
    }

    //deal with global variables
    free_list_ptr = (vaddr_t)0;
    memory_total = memory_size = size;
    strategy = BEST_FIT;

    //create first free_block header
    free_header_t *fp = (void *)memory;
    fp->magic = MAGIC_FREE;
    fp->size = size;
    fp->next = fp->prev = 0;
    return;
}


// Input: n - number of bytes requested
// Output: p - a pointer, or NULL
// Precondition: n < size of largest available free block
// Postcondition: If a region of size n or greater cannot be found, p = NULL
//                Else, p points to a location immediately after a header block
//                      for a newly-allocated region of some size >=
//                      n + header size.

void *vlad_malloc(u_int32_t n)
{
    // TODO for Milestone 2
    if (n > memory_size){
        fprintf(stderr, "vlad_malloc: Insufficient memory\n");
        return NULL;
    }

    free_header_t *first_free = (void *)(memory + free_list_ptr);
    if (first_free->magic != MAGIC_FREE){
        fprintf(stderr, "vlad_malloc1: Memory corruption\n");
        exit(EXIT_FAILURE);
    }

    free_header_t *curr = first_free;
    vsize_t alloc_size;
    vsize_t best_size = memory_size;
    vaddr_t best_addr = free_list_ptr;
    int flag = 0;

    //calculate alloc_size
    if (n < 8) n = 8;
    if (n % 4 == 0) {alloc_size = n + ALLOC_HEADER_SIZE;}
    else {alloc_size = n + 4 - n%4 + ALLOC_HEADER_SIZE;}

    //search for a possible free_block
    do {
        if (curr->magic != MAGIC_FREE){
            fprintf(stderr, "vald_malloc2: Memory corruption\n");
            exit(EXIT_FAILURE);
        }

        if (curr->size >= alloc_size){
            best_size = curr->size;
            best_addr = (void*)curr - (void*)memory;
            flag = 1;
            break;
        }
        curr = (void*)(memory + curr->next);
    } while (curr != first_free);

    //if no suitable free_block
    if (flag == 0){
        printf("vlad_malloc: Insufficient memory to allocate\n");
        return NULL;
    }

    //search for BEST_ALLOC
    do {
        if (curr->magic != MAGIC_FREE){
            fprintf(stderr, "vald_malloc3: Memory corruption\n");
            exit(EXIT_FAILURE);
        }

        if (curr->size < best_size && curr->size >= alloc_size){
            best_size = curr->size;
            best_addr = (void*)curr - (void*)memory;
        }
        curr = (void*)(memory + curr->next);
    } while (curr != first_free);

    //deal with free_block_header
    free_header_t *old = (void *)(memory + best_addr);
    if (old->magic != MAGIC_FREE){
        fprintf(stderr, "vald_alloc4: Memory corruption\n");
        exit(EXIT_FAILURE);
    }

    free_header_t *old_prev = (void *)memory + old->prev;
    free_header_t *old_next = (void *)memory + old->next;

    if (old_prev->magic != MAGIC_FREE){
        fprintf(stderr, "vlad_malloc5: Memory corruption\n");
        exit(EXIT_FAILURE);
    }

    if (old_next->magic != MAGIC_FREE){
        fprintf(stderr, "vlad_malloc6: Memory corruption\n");
        exit(EXIT_FAILURE);
    }

    //deal with fragmentation -- delete old free_block
    if (old->size < alloc_size + 2 * FREE_HEADER_SIZE){

        //only one free_block
        if(old->next == ((void*)old - (void*)memory)){
            printf("vlad_malloc: Insufficient memory to allocate\n");
            return NULL;
        }           //no memory to put a free_block header
        //more than one free_blocks
        else{
            alloc_size = old->size;
            old_next->prev = (void *)old_prev - (void *)memory;
            old_prev->next = (void *)old_next - (void *)memory;
        }

        if (best_addr == free_list_ptr) free_list_ptr = old_prev->next;

    }

    //no fragmentation -- move old free_block_header to a new one
    else {
        free_header_t *new = (void *)(memory + best_addr + alloc_size);
        new->magic = MAGIC_FREE;
        new->size = old->size - alloc_size;

        //only one free_block
        if(old->next == ((void*)old - (void*)memory)){
            new->next = old->next + alloc_size;
            new->prev = old->prev + alloc_size;
        }
        //more than one free_blocks
        else{
            new->prev = (void *)old_prev - (void *)memory;
            new->next = (void *)old_next - (void *)memory;
            old_prev->next = (void *)new -(void *)memory;
            old_next->prev = (void *)new -(void *)memory;
        }

        if (best_addr == free_list_ptr) free_list_ptr += alloc_size;
    }

    //printf("best_addr is %d\n", best_addr);       //for debugging

    //alloc in best_addr
    alloc_header_t *new = (void *)(memory + best_addr);
    new->size = alloc_size;
    new->magic = MAGIC_ALLOC;

    memory_size -= new->size;

    vlad_stats();
    return ((void *)new + ALLOC_HEADER_SIZE);
}


// Input: object, a pointer.
// Output: none
// Precondition: object points to a location immediately after a header block
//               within the allocator's memory.
// Postcondition: The region pointed to by object has been placed in the free
//                list, and merged with any adjacent free blocks; the memory
//                space can be re-allocated by vlad_malloc

void vlad_free(void *object)
{

    // TODO for Milestone 3
    if (object  - ALLOC_HEADER_SIZE < (void *)memory || object  - ALLOC_HEADER_SIZE > (void *)memory + memory_total){
        fprintf(stderr, "vlad_free: Attempt to free via invalid pointer\n");
        exit(EXIT_FAILURE);
    }

    alloc_header_t *old = (void*)object - ALLOC_HEADER_SIZE;
    u_int32_t index = (void *)old - (void*)memory;

    if (old->magic != MAGIC_ALLOC ){
        fprintf(stderr, "vlad_free: Attempt to free non-allocated memory\n");
        exit(EXIT_FAILURE);
    }

    //create free_header to replace alloc_header
    free_header_t *new = object - ALLOC_HEADER_SIZE;
    new->magic = MAGIC_FREE;
    new->size = old->size;
    //printf("size of new is %d\n", new->size);     //for debugging

    //insert a new free_block to the free_block linked list
    free_header_t *first_free = (void *)(memory + free_list_ptr);
    free_header_t *curr = first_free;

    //search for correct position
    while (index > ((void*)curr - (void*)memory)){

        if (curr->magic != MAGIC_FREE){
            fprintf(stderr, "vlad_free1: Memory corruption\n");
            exit(EXIT_FAILURE);
        }

        curr = (void*)(memory + curr->next);
        if (curr == first_free) break;
    }

    //only one free_block
    if (curr->next == ((void*)curr - (void*)memory)){
        new->prev = new->next = free_list_ptr;
        curr->next = curr->prev = index;
    }
    //more than one free_blocks
    else{
        new->prev = curr->prev;
        new->next = (void*)curr - (void *)memory;
        free_header_t *curr_prev = (void *)memory + curr->prev;

        if (curr_prev->magic != MAGIC_FREE){
            fprintf(stderr, "vlad_free2: Memory corruption\n");
            exit(EXIT_FAILURE);
        }

        curr->prev = index;
        curr_prev->next = index;

    }

    //index becomes first_list_ptr
    if (index < free_list_ptr) free_list_ptr = index;

    memory_size += new->size;
    //printf("Next of free'd block is %d\nPrevious of free'd block is %d\n", new->next, new->prev);     //for debugging

    vlad_stats();
    vlad_merge();

}

// Input: current state of the memory[]
// Output: new state, where any adjacent blocks in the free list
//            have been combined into a single larger block; after this,
//            there should be no region in the free list whose next
//            reference is to a location just past the end of the region

static void vlad_merge()
{
	// TODO for Milestone 4
    free_header_t *first_free = (void *)(memory + free_list_ptr);
    free_header_t *curr = first_free;
    int flag = 0;



        do {
            if (first_free->magic != MAGIC_FREE){
                fprintf(stderr, "vlad_merge1: Memory corruption\n");
                exit(EXIT_FAILURE);
            }
            //search for free_block needed to merge
            if ((void *)curr - (void *)memory + curr->size == curr->next){
                free_header_t *delete = (void *)curr + curr->size;
                free_header_t *delete_next = (void *)memory + delete->next;

                if (delete->magic != MAGIC_FREE){
                    fprintf(stderr, "vlad_merge2: Memory corruption\n");
                    exit(EXIT_FAILURE);
                }

                if (delete_next->magic != MAGIC_FREE){
                    fprintf(stderr, "vlad_merge3: Memory corruption\n");
                    exit(EXIT_FAILURE);
                }

                //merging
                delete_next->prev = (void *)curr - (void *)memory;
                curr->next = delete->next;
                curr->size += delete->size;

                //printf("after merging:\n");
                //printf("Next of free'd block is %d\nPrevious of free'd block is %d\n", curr->next, curr->prev);       //for debugging
                flag = 1;
                break;
            }

        curr = (void*)(memory + curr->next);

        } while (curr != first_free);

    if (flag == 0)  return;
    else vlad_merge();


}

// Stop the allocator, so that it can be init'ed again:
// Precondition: allocator memory was once allocated by vlad_init()
// Postcondition: allocator is unusable until vlad_int() executed again

void vlad_end(void)
{
    free(memory);

    return;
   // TODO for Milestone 1
}


// Precondition: allocator has been vlad_init()'d
// Postcondition: allocator stats displayed on stdout

void vlad_stats(void)
{
    printf("free_list_ptr %d\nmemory_size is %d\n", free_list_ptr, memory_size);
    // TODO
    // put whatever code you think will help you
    // understand Vlad's current state in this function
    // REMOVE all of these statements when your vlad_malloc() is done


    //print all free block
    /*printf("free_list_ptr %d\n", free_list_ptr);
    free_header_t *first_free = (void *)(memory + free_list_ptr);
    free_header_t *curr;
    int i = 1;
    curr = first_free;
    do {
    printf("free block %d, size is %d, previous is %d and next is %d\n", i, curr->size, curr->prev, curr->next);
    curr = (void *)(memory + curr->next);
    ++i;
    } while (curr != first_free);
    */

    /*printf("vlad_stats() won't work until vlad_malloc() works\n"); */
    return;
}

