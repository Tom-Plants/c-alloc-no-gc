typedef void h_zone_ctx; // handle of zone

/**
 * return 0 我滴任务完成啦，啊哈哈哈哈
 * return 1 关我批事，请加速
 */
typedef enum _MEM_REQUIREMENT {
  MEM_RECLAIM = 0,
  MEM_DONT_FREE = 1
} MEM_REQUIREMENT;

typedef MEM_REQUIREMENT (*fp_reclaiming)();

void *create_zone(unsigned long size);
void free_zone(h_zone_ctx *zone);
void *alloc_from_zone(h_zone_ctx *zone, unsigned long size,
                      fp_reclaiming callback);
void print_zone(h_zone_ctx *zone);
