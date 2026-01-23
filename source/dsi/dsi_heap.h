#ifndef __DSI_HEAP_H__
#define __DSI_HEAP_H__

#include <stddef.h>

void DSI_InitHeap(void);
void *DSI_Malloc(size_t size);
void DSI_Free(void *ptr);

#endif
