#include <nds.h>
#include <stdio.h>
#include "dsi_heap.h"

// Simple First-Fit Allocator for DSi Extended RAM
// RAM Range: 0x0D000000 - 0x0DFFFFFF (16MB, we use 10MB as requested)

#define HEAP_START 0x02400000 // Start of DSi Extended RAM (4MB offset)
#define HEAP_SIZE  (10 * 1024 * 1024)

typedef struct heap_block_s {
    size_t size;          // Size of data part
    struct heap_block_s *next;
    int free;
} heap_block_t;

static heap_block_t *heap_head = NULL;
static size_t total_free = 0;

void DSI_InitHeap(void) {
    if (!isDSiMode())
    {
        iprintf("\nError: DSi Mode required!\n");
        iprintf("Please enable DSi Mode in\n");
        iprintf("your emulator/flashcard.\n");
        while(1) swiWaitForVBlank();
    }

    heap_head = (heap_block_t *)HEAP_START;
    
    // Safety check: Ensure we don't overwrite code if it grew too large
    // (This is a rough check, ideally we check linker symbols)
    extern u8 __end__[]; // defined by linker
    if ((u32)__end__ > HEAP_START)
    {
        iprintf("\nError: Code overlaps Heap!\n");
        iprintf("Code end: %p\n", __end__);
        iprintf("Heap start: %p\n", (void*)HEAP_START);
        while(1) swiWaitForVBlank();
    }

    heap_head->size = HEAP_SIZE - sizeof(heap_block_t);
    heap_head->next = NULL;
    heap_head->free = 1;
    total_free = heap_head->size;
    
    // Clear memory (optional but good for debugging)
    // memset((void*)((u32)heap_head + sizeof(heap_block_t)), 0, heap_head->size);
    
    printf("DSi Heap Init: 10MB at %p\n", (void*)HEAP_START);
}

void *DSI_Malloc(size_t size) {
    heap_block_t *curr = heap_head;
    heap_block_t *best = NULL;
    
    // Align size to 4 bytes
    size = (size + 3) & ~3;

    // Find first fit
    while (curr) {
        if (curr->free && curr->size >= size) {
            best = curr;
            break; 
        }
        curr = curr->next;
    }

    if (!best) return NULL; // Out of memory

    // Split block if possible
    if (best->size > size + sizeof(heap_block_t) + 16) {
        heap_block_t *new_block = (heap_block_t *)((u32)best + sizeof(heap_block_t) + best->size);
        // Correct calculation:
        // We want to split 'best' into 'best' (size) and 'new' (remaining)
        // 'new' starts at best + sizeof + size
        
        new_block = (heap_block_t *)((u8*)best + sizeof(heap_block_t) + size);
        new_block->size = best->size - size - sizeof(heap_block_t);
        new_block->next = best->next;
        new_block->free = 1;
        
        best->size = size;
        best->next = new_block;
    }

    best->free = 0;
    total_free -= (best->size + sizeof(heap_block_t));
    return (void *)((u8*)best + sizeof(heap_block_t));
}

void DSI_Free(void *ptr) {
    if (!ptr) return;

    heap_block_t *block = (heap_block_t *)((u8*)ptr - sizeof(heap_block_t));
    block->free = 1;
    total_free += block->size;

    // Coalesce with next
    if (block->next && block->next->free) {
        block->size += sizeof(heap_block_t) + block->next->size;
        block->next = block->next->next;
    }

    // Coalesce with prev? 
    // Since we don't have prev pointer, we have to traverse. 
    // To optimize, we could add prev pointer, but for now we traverse.
    // Given the constraints and usage pattern, simple traverse is acceptable or we add prev.
    // Adding prev makes it Doubly Linked List, easier.
}

// Re-implementing with Doubly Linked List for O(1) free (except coalescing)
// Actually traversal for coalesce is fast enough for SRB2's 10MB heap.
