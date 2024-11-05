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
    printf("strtab size = %ld\n", sizeof(strtab));
  assert(ret_strtab == 1);

  //计算符号数量并输出符号信息
int num_symbols = symtab_section->sh_size / sizeof(Elf32_Sym);
printf("entry = %d\n", num_symbols);
    for (int i = 0; i < num_symbols; i++) {
            printf("%d Symbol: %s, TYPE: %d Address: 0x%x, Size: %u\n",
                   i,
                   &strtab[symtab[i].st_name],
                   ELF32_ST_TYPE(symtab[i].st_info),
                   symtab[i].st_value,
                   symtab[i].st_size);
        }
    // 释放分配的内存并关闭文件
    free(section_headers);
    free(symtab);
    free(strtab);
    fclose(fp);
}
#else
void init_ftrace(const char *elf_file){ }
#endif
