#include "zone.h"
#include <stdio.h>
#include <string.h>

MEM_REQUIREMENT reclaim() {
  printf("reclaim1 的任务还未完成，继续持有该块内存\n");
  return MEM_DONT_FREE;
}
MEM_REQUIREMENT reclaim2() {
  printf("reclaim2 的任务还未结束，该内存可以被释放\n");
  return MEM_RECLAIM;
}
MEM_REQUIREMENT reclaim3() {
  printf("reclaim3 的任务已经结束，该内存可以被释放\n");
  return MEM_RECLAIM;
}
MEM_REQUIREMENT reclaim4() {
  printf("reclaim4 的任务已经结束，该内存可以被释放\n");
  return MEM_RECLAIM;
}
MEM_REQUIREMENT reclaim5() {
  printf("reclaim5 的任务已经结束，该内存可以被释放\n");
  return MEM_RECLAIM;
}
MEM_REQUIREMENT reclaim6() {
  printf("reclaim6 的任务已经结束，该内存可以被释放\n");
  return MEM_RECLAIM;
}
MEM_REQUIREMENT reclaim7() {
  printf("reclaim7 的任务已经结束，该内存可以被释放\n");
  return MEM_RECLAIM;
}
int main() {
  void *zone1 = create_zone(100);
  print_zone(zone1);

  alloc_from_zone(zone1, 10, reclaim);
  print_zone(zone1);

  alloc_from_zone(zone1, 10, reclaim2);
  print_zone(zone1);

  alloc_from_zone(zone1, 10, reclaim3);
  print_zone(zone1);

  void *g = alloc_from_zone(zone1, 1, reclaim4);
  memset(g, 90, 1);
  print_zone(zone1);

  void *k = alloc_from_zone(zone1, 10, reclaim5);
  memset(k, 2, 10);
  print_zone(zone1);

  void *p = alloc_from_zone(zone1, 10, reclaim6);
  memset(p, 1, 10);
  print_zone(zone1);

  void *i = alloc_from_zone(zone1, 10, reclaim6);
  memset(i, 3, 10);
  print_zone(zone1);

  free_zone(zone1);
  return 0;
}
