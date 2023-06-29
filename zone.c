#include "zone.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define POINTER_SIZE sizeof(long)
#define MAX_ALLOC_COUNT 1024

void print_mem(const void *ptr, size_t size) {
  const unsigned char *mem = (const unsigned char *)ptr;
  int p = 0;
  for (size_t i = 0; i < size; i++) {
    if (p != 0 && p % 8 == 0)
      printf("|");
    if ((p++) % 32 == 0)
      printf("\n");
    printf("%02X ", mem[i]);
  }
  printf("\n");
}

void print_zone(zone_ctx *zone) {
  printf("--------------------\n");
  printf("zone: offset: %ld \n", zone->offset);
  print_mem(zone->base, zone->size);
  printf("zone_manager: offset: %ld\n", zone->manager_offset);
  print_mem(zone->manager_base, 10 * POINTER_SIZE);
  printf("--------------------\n");
}
/**
 * size -> byte
 */
void *create_zone(unsigned long size) {
  // zone_ctx ctx;
  zone_ctx *ctx = malloc(sizeof(zone_ctx));

  ctx->size = size;
  ctx->offset = 0;
  ctx->base = malloc(size);

  //最大有1024个堆申请,超过将回滚
  ctx->manager_base = malloc(POINTER_SIZE * MAX_ALLOC_COUNT);
  memset(ctx->manager_base, 0, MAX_ALLOC_COUNT);

  return ctx;
}

void free_zone(zone_ctx *zone) {
  free(zone->base);
  free(zone);
}

void next_page(zone_ctx *zone) {
  unsigned int size = 0;

  size = *(unsigned long *)((char *)zone->base + zone->offset);

  zone->offset += POINTER_SIZE * 2 + size;
  zone->manager_offset += 1;
}

int clean_pages(zone_ctx *zone, unsigned long size) {

  void **page = NULL;
  void **page_reclaim_fp = NULL;
  int res = 0;

  zone_ctx ctx;
  memcpy(&ctx, zone, sizeof(zone_ctx));

  while (1) {
    page = (void **)(((unsigned long *)ctx.manager_base) + ctx.manager_offset);
    if (*page == NULL) {
      return 0;
    }
    if ((long)*page == -1) {
      next_page(&ctx);
      continue;
    }

    // printf("required: %lX, page .. %lX\n", (long)ctx.base + ctx.offset +
    // size, (long)*page);

    if (*page < (void *)((char *)ctx.base + ctx.offset + size)) {
      page_reclaim_fp = (void **)(((unsigned long *)*page) + 1);
      fp_reclaiming fp = (*page_reclaim_fp);
      res = fp();
      if (res > 0)
        return 1;
      *((long *)ctx.manager_base + ctx.manager_offset) = -1;
      unsigned long inital_offset = ctx.offset;
      next_page(&ctx);
      ctx.offset = inital_offset; // dont change offset
      continue;
    } else {
      return 0;
    }
  }
}

void *create_page(zone_ctx *zone, unsigned long size, fp_reclaiming callback) {

  void *page_content = NULL;
  // zone
  *(unsigned long *)((char *)zone->base + zone->offset) = size;

  *(unsigned long *)(zone->base + zone->offset + POINTER_SIZE) =
      (unsigned long)callback;

  *((unsigned long *)(zone->manager_base) + zone->manager_offset) =
      (unsigned long)(zone->base + zone->offset);

  page_content = zone->base + zone->offset + POINTER_SIZE * 2;

  zone->offset += POINTER_SIZE * 2 + size;
  // zone_manager
  zone->manager_offset += 1;

  return page_content;
}

void *alloc_from_zone(zone_ctx *zone, unsigned long size,
                      fp_reclaiming callback) {

  size = ((size / 4) + 1) * 4; // mem align

  if (POINTER_SIZE * 2 + size > zone->size) {
    printf(" -- alloc memory oversize --\n");
    return NULL;
  }

  long try_count = 0;

NEXT_PAGE:
  try_count++;

  if (try_count > MAX_ALLOC_COUNT) {
    printf("[warn] no free memory !\n");
    return NULL;
  }

  if (POINTER_SIZE * 2 + size + zone->offset > zone->size) {
    zone->offset = 0;
    zone->manager_offset = 0;
  }

  if (clean_pages(zone, size) == 0) {
    return create_page(zone, size, callback);
  } else {
    // printf("%lX report DONT_FREE, goto next page \n",
    //(long)((char *)zone->base + zone->offset));
    next_page(zone);
    goto NEXT_PAGE;
  }
}
