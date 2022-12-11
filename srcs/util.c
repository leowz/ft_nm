/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:57:09 by zweng             #+#    #+#             */
/*   Updated: 2022/12/11 19:34:54 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int check_elf_ident(char *file, unsigned char *arch, size_t size)
{
    if (size < 16)
        return (FUN_FAIL);
    *arch = file[EI_CLASS];
    if (ft_memcmp(file, ELFMAG, SELFMAG) != 0 || 
            file[EI_CLASS] <= 0 || file[EI_CLASS] > 2 ||
            file[EI_DATA] <= 0 || file[EI_DATA] > 2 ||
            file[EI_VERSION] != EV_CURRENT)
        return (0);
    else
        return (1);
}

int is_special_section_indice(uint16_t s_idx)
{
    return (s_idx == SHN_LOPROC || s_idx == SHN_BEFORE || s_idx == SHN_AFTER ||
            s_idx == SHN_UNDEF || s_idx == SHN_HIPROC || s_idx == SHN_LOOS ||
            s_idx == SHN_HIOS || s_idx == SHN_ABS || s_idx == SHN_COMMON || 
            s_idx == SHN_XINDEX || s_idx == SHN_HIRESERVE);
}

static char     get_sym_type1(const char *symname, int symbind, int symtype,
        unsigned long addr)
{

}

unsigned char   get_sym_type(const char *sname, int symbind, int symtype, unsigned long addr)
{
      int c = '.';
      int i;
      s_nmTypename nmtype[15] = {
      {'b', ".bss\0"},
      {'t', ".text\0"},
      {'t', ".init\0"},
      {'t', ".fini\0"},
      {'d', ".data\0"},
      {'d', ".got.plt\0"},
      {'d', ".init_array\0"},
      {'d', ".dynamic\0"},
      {'d', ".fini_array\0"},
      {'r', ".rodata\0"},
      {'r', ".eh_frame\0"},
      {'r', ".eh_frame_hdr\0"},
      {'r', ".gcc_except_table\0"},
      {'r', ".interp\0"},
      };

  if (symbind == STB_WEAK)
    return (('W' * (addr != 0) + 'w' * (!addr)) *
            (symtype == STT_FUNC || symtype == STT_NOTYPE)) +
           (('V' * (addr != 0) + 'v' * (!addr)) * (symtype == STT_OBJECT));
  if (!*sname)
    c = 'u';
  else
    for (i = 0; i < 15; i++) {
      if (!ft_strncmp(sname, nmtype[i].sectionName,
                      ft_strlen(nmtype[i].sectionName) + 1)) {
        c = nmtype[i].nmType;
        break;
      }
    }
  if (c == '.')
    if (!ft_strncmp(sname, ".rodata", 7)) c = 'r';
  return ((c * (symbind == STB_LOCAL)) + ((c & '_') * (symbind == STB_GLOBAL)));
}

void            set_sym_arr(t_symbol *arr, unsigned int index,
        unsigned int value, unsigned char type, char *name)
{
    if (arr && (arr + index))
    {
        arr[index].value = value;
        arr[index].type = type;
        arr[index].name = name;
    }
}
