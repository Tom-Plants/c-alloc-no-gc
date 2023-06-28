#include "zone.h"
#include <stdio.h>

MEM_REQUIREMENT reclaim() {
  printf("reclaim1 ... 关我批事，请加速\n");
  return MEM_DONT_FREE;
}
MEM_REQUIREMENT reclaim2() {
  printf("reclaim2 ... 我滴任务，完成啦，啊哈哈哈哈\n");
  return MEM_RECLAIM;
}
MEM_REQUIREMENT reclaim3() {
  printf("reclaim2 ... 我滴任务，完成啦，啊哈哈哈哈\n");
  return MEM_RECLAIM;
}
int main() {
  void *zone1 = create_zone(100);
  print_zone(zone1);

  alloc_from_zone(zone1, 10, reclaim);
  print_zone(zone1);

  alloc_from_zone(zone1, 10, reclaim2);
  print_zone(zone1);

  alloc_from_zone(zone1, 10, reclaim2);
  print_zone(zone1);

  alloc_from_zone(zone1, 50, reclaim3);
  print_zone(zone1);

  free_zone(zone1);
  return 0;
}
