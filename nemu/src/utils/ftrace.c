/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/nemu/src/utils/ftrace.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年11月05日 星期二 12时11分04秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<debug.h>
#include<elf.h>

#ifdef CONFIG_FTRACE
typedef struct{
  Elf32_Sym *filtered_symtab;
  char *strtab;
  int num_func_symbols;
} FTraceData;

FTraceData *ftracedata = NULL;

void init_ftrace(const char *elf_file){
  Assert(elf_file, "elf_file is null, input elf_file using '--elf=[filename].elf'");
  FILE *fp = fopen(elf_file, "rb");
  Assert(fp, "Can not open '%s'", elf_file);

  Elf32_Ehdr elf_header;
  int ret_elf_hdr = fread(&elf_header, sizeof(Elf32_Ehdr), 1, fp);
  assert(ret_elf_hdr == 1);

  //elf文件检查。只有前面四个字节为0x7F, 'E', 'L', 'F'，即ELFMAG数组，才表示为elf文件格式
  if(memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0){ 
    fclose(fp);
    Assert(fp, "'%s' is not a valid elf file", elf_file);
  }

  //跳到Section Header table
  fseek(fp, elf_header.e_shoff, SEEK_SET);

  //从elf头的信息获取节头表到内存中
  Elf32_Shdr *section_headers = malloc(elf_header.e_shnum * elf_header.e_shentsize);
  int ret_sec_hdr = fread(section_headers, elf_header.e_shentsize, elf_header.e_shnum, fp);
  assert(ret_sec_hdr == elf_header.e_shnum);

  //由节头表的信息找到指向节头表中符号表和字符串表的指针
  Elf32_Shdr *symtab_section = NULL;
  Elf32_Shdr *strtab_section = NULL;

  for(int i = 0; i < elf_header.e_shnum; i++){
    //获取节头表中指向符号表的指针
    if(section_headers[i].sh_type == SHT_SYMTAB){
      symtab_section = &section_headers[i];
    }

    //获取节头表中指向字符串表的指针
    if(section_headers[i].sh_type == SHT_STRTAB &&  i != elf_header.e_shstrndx) //排除节头字符串表
      strtab_section = &section_headers[i];
  }
  
  if (!symtab_section || !strtab_section){
    fprintf(stderr, "Failed to find symbol or string table in the ELF file\n");
    free(section_headers);
    fclose(fp);
  }

  //读取符号表
  Elf32_Sym *symtab = malloc(symtab_section->sh_size);
  fseek(fp, symtab_section->sh_offset, SEEK_SET);
  int ret_symtab = fread(symtab, symtab_section->sh_size, 1, fp);
  assert(ret_symtab == 1);

  //读取字符串表
  char *strtab = malloc(strtab_section->sh_size);
  fseek(fp, strtab_section->sh_offset, SEEK_SET);
  int ret_strtab = fread(strtab, strtab_section->sh_size, 1, fp);
  assert(ret_strtab == 1);

  int num_func_symbols = 0;
  int num_symbols = symtab_section->sh_size / sizeof(Elf32_Sym);

  //先计算type为func的symbol数量，方便后面分配空间
  for(int i = 0; i < num_symbols; i++){
    if(ELF32_ST_TYPE(symtab[i].st_info) == STT_FUNC){
      num_func_symbols++;
    }
  }

  //分配内存用于存储type为STT_FUNC的symbol
  Elf32_Sym *filtered_symtab = malloc(num_func_symbols * sizeof(Elf32_Sym));
  int idx = 0;
  for(int i = 0; i < num_symbols; i++){
    if(ELF32_ST_TYPE(symtab[i].st_info) == STT_FUNC){
      filtered_symtab[idx++] = symtab[i];
    }
  }

  //创建并填充FTraceData结构体
  ftracedata = malloc(sizeof(FTraceData));
  ftracedata->filtered_symtab = filtered_symtab;
  ftracedata->strtab = strtab;
  ftracedata->num_func_symbols = num_func_symbols;


  //计算符号数量并输出符号信息
    for (int i = 0; i < ftracedata->num_func_symbols; i++) {
            printf("%d Symbol: %s, TYPE: %d Address: 0x%x, Size: %u\n",
                   i,
                   &ftracedata->strtab[ftracedata->filtered_symtab[i].st_name],
                   ELF32_ST_TYPE(ftracedata->filtered_symtab[i].st_info),
                   ftracedata->filtered_symtab[i].st_value,
                   ftracedata->filtered_symtab[i].st_size);
    }

    // 释放分配的内存并关闭文件
    free(section_headers);
    free(symtab);
    fclose(fp);
}
void cleanup_ftrace(){
  if(ftracedata){
    free(ftracedata->filtered_symtab);
    free(ftracedata->strtab);
    free(ftracedata);
  }
}

int n = 0;

void func_call(vaddr_t pc, vaddr_t jal_addr){
  char *func_call_name = NULL;
  for (int i = 0; i < ftracedata->num_func_symbols; i++) {
    if(jal_addr >= ftracedata->filtered_symtab[i].st_value && jal_addr < (ftracedata->filtered_symtab[i].st_value + ftracedata->filtered_symtab[i].st_size)){
      func_call_name = &ftracedata->strtab[ftracedata->filtered_symtab[i].st_name];
      n++;
      printf("0x%x: ", pc);
      for(int j = 0; j < n; j++){
        printf(" ");
      }
      printf("call [%s@0x%x]\n", func_call_name, jal_addr);
    }
  }
}
void func_ret(vaddr_t pc, vaddr_t jalr_addr){
  char *func_ret_name = NULL;
  for (int i = 0; i < ftracedata->num_func_symbols; i++) {
    if(jalr_addr >= ftracedata->filtered_symtab[i].st_value && jalr_addr < (ftracedata->filtered_symtab[i].st_value + ftracedata->filtered_symtab[i].st_size)){
      func_ret_name = &ftracedata->strtab[ftracedata->filtered_symtab[i].st_name];
      n--;
      printf("0x%x: ", pc);
      for(int j = 0; j < n; j++){
        printf(" ");
      }
      printf("ret [%s]\n", func_ret_name);
    }
  }
}
#else
void init_ftrace(const char *elf_file){ }
void func_call(vaddr_t pc, vaddr_t jal_addr){ }
void func_ret(vaddr_t pc, vaddr_t jalr_addr){ }
#endif
