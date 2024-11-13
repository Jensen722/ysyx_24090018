/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <device/map.h>
#include <memory/paddr.h>

#define NR_MAP 16

static IOMap maps[NR_MAP] = {};
static int nr_map = 0;

//给定一个物理地址，返回是属于第几个map的
static IOMap* fetch_mmio_map(paddr_t addr) {
  int mapid = find_mapid_by_addr(maps, nr_map, addr);
  printf("mapid: %d\n", mapid);
  return (mapid == -1 ? NULL : &maps[mapid]);
}

//当内存空间重叠时打印输出
static void report_mmio_overlap(const char *name1, paddr_t l1, paddr_t r1,
    const char *name2, paddr_t l2, paddr_t r2) {
  panic("MMIO region %s@[" FMT_PADDR ", " FMT_PADDR "] is overlapped "
               "with %s@[" FMT_PADDR ", " FMT_PADDR "]", name1, l1, r1, name2, l2, r2);
}

/* device interface */
void add_mmio_map(const char *name, paddr_t addr, void *space, uint32_t len, io_callback_t callback) { //在已有的map中添加新的mmio map
  assert(nr_map < NR_MAP);
  paddr_t left = addr, right = addr + len - 1;
  if (in_pmem(left) || in_pmem(right)) { //检查需要新添加的内存空间是否已与存在的pmem空间重叠
    report_mmio_overlap(name, left, right, "pmem", PMEM_LEFT, PMEM_RIGHT);
  }
  for (int i = 0; i < nr_map; i++) {  //检查需要新添加的内存空间是否已与存在的mmio map空间重叠
    if (left <= maps[i].high && right >= maps[i].low) {
      report_mmio_overlap(name, left, right, maps[i].name, maps[i].low, maps[i].high);
    }
  }

  maps[nr_map] = (IOMap){ .name = name, .low = addr, .high = addr + len - 1,
    .space = space, .callback = callback }; //添加mmio map
  Log("Add mmio map '%s' at [" FMT_PADDR ", " FMT_PADDR "]",  //打印输出
      maps[nr_map].name, maps[nr_map].low, maps[nr_map].high);

  nr_map ++;
}

/* bus interface */
word_t mmio_read(paddr_t addr, int len) {
  return map_read(addr, len, fetch_mmio_map(addr));
}

void mmio_write(paddr_t addr, int len, word_t data) {
  map_write(addr, len, data, fetch_mmio_map(addr));
}
