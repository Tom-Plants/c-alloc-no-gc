#include "zone.h"
#include <stdio.h>
#include <string.h>

MEM_REQUIREMENT reclaim() {
  printf("reclaim1 的任务还未完成，继续持有该块内存\n");
  return MEM_DONT_FREE;
}
MEM_REQUIREMENT reclaim2() {
  printf("reclaim2 的任务已经结束，该内存可以被释放\n");
  return MEM_RECLAIM;
}
MEM_REQUIREMENT reclaim3() {
  printf("reclaim3 的任务已经结束，该内存可以被释放\n");
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

  alloc_from_zone(zone1, 1, reclaim3);
  print_zone(zone1);

  void *k = alloc_from_zone(zone1, 50, reclaim3);
  memset(k, 0, 50);
  print_zone(zone1);

  free_zone(zone1);
  return 0;
}
