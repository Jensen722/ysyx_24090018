/*************************************************************************
	> File Name: csrc/monitor/monitor.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 11时11分29秒
 ************************************************************************/

#include<stdio.h>
#include<assert.h>
#include<getopt.h>
#include<stdlib.h>

int *guest_to_host(int addr);
#define RESET_VECTOR 0x80000000

static char *log_file = NULL;
static char *elf_file = NULL;
static char *diff_so_file = NULL;
static char *img_file = NULL;
static int difftest_port = 1234;

static long load_img() {
  if (img_file == NULL) {
    printf("img_file is NULL\n");
fflush(stdout); 
    assert(0);
    return 4096; // built-in image size
  }

  FILE *fp = fopen(img_file, "rb");
  //Assert(fp, "Can not open '%s'", img_file);
  assert(fp);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);

  //Log("The image is %s, size = %ld", img_file, size);

  fseek(fp, 0, SEEK_SET);
  printf("%x\n",guest_to_host(RESET_VECTOR));
  int ret = fread(guest_to_host(RESET_VECTOR), size, 1, fp);
  assert(ret == 1);

  fclose(fp);
  return size;
}

static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"batch"    , no_argument      , NULL, 'b'},
    {"log"      , required_argument, NULL, 'l'},
    {"diff"     , required_argument, NULL, 'd'},
    {"elf"      , required_argument, NULL, 'e'},
    {"port"     , required_argument, NULL, 'p'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL,  0 },
  };
  int o;
  while ( (o = getopt_long(argc, argv, "-bhl:d:p:e:", table, NULL)) != -1) {
    switch (o) {
      //case 'b': sdb_set_batch_mode(); break;
      case 'p': sscanf(optarg, "%d", &difftest_port); break;
      case 'l': log_file = optarg; break;
      case 'd': diff_so_file = optarg; break;
      case 'e': elf_file = optarg; break;
      case 1: img_file = optarg; return 0;
      default:
        printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
        printf("\t-b,--batch              run with batch mode\n");
        printf("\t-l,--log=FILE           output log to FILE\n");
        printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
        printf("\t-e,--elf=ELF            ftrace with elf file\n");
        printf("\t-p,--port=PORT          run DiffTest with port PORT\n");
        printf("\n");
        exit(0);
    }
  }
  return 0;
}
void init_mem();
int pmem_read(int addr);
void init_monitor(int argc, char *argv[]) {
  /* Perform some global initialization. */

  /* Parse arguments. */
  parse_args(argc, argv);

  /* Initialize memory. */
  init_mem();

  /* Load the image to memory. This will overwrite the built-in image. */
  long img_size = load_img();
  printf("img_size = %ld\n", img_size);
  //printf("inst = 0x%08x\n", pmem_read(0x80000000));
}

