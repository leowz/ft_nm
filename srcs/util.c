/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:57:09 by zweng             #+#    #+#             */
/*   Updated: 2023/01/08 17:56:55 by zweng            ###   ########.fr       */
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
            s_idx == SHN_HIPROC || s_idx == SHN_LOOS || s_idx == SHN_HIOS ||
            s_idx == SHN_ABS || s_idx == SHN_COMMON || s_idx == SHN_XINDEX ||
            s_idx == SHN_HIRESERVE);
}

unsigned char	get_ssi_type(unsigned int s_idx)
{
	if (s_idx == SHN_UNDEF)
		return ('u');
	if (s_idx == SHN_COMMON)
		return ('c');
	else
		return ('a');
}

unsigned char   get_sym_type(const char *sname, int symbind, int symtype,
		unsigned long addr)
{
      int c;
      int i;
      s_nmTypename nmtype[20] = {
      {'b', ".bss"},
      {'d', ".data"},
      {'d', ".got"},
      {'d', ".init_array"},
      {'d', ".fini_array"},
      {'d', ".dynamic"},
      {'r', ".rodata"},
      {'r', ".eh_frame"},
      {'r', ".gcc_except_table"},
      {'r', ".interp"},
      {'r', ".note"},
      {'r', ".dynsym"},
      {'r', ".dynstr"},
      {'r', ".gnu"},
      {'r', ".rela"},
      {'n', ".comment"},
      {'t', ".text"},
      {'t', ".init"},
      {'t', ".fini"},
      {'t', ".plt"},};

	  if (symbind == STB_WEAK)
		  return (('W' * (addr != 0) + 'w' * (!addr)) *
				  (symtype == STT_FUNC || symtype == STT_NOTYPE)) +
			  (('V' * (addr != 0) + 'v' * (!addr)) * (symtype == STT_OBJECT));
	  if (!*sname)
		  c = 'u';
	  else
	  {
		  i = 0;
		  while (i < 20)
		  {
			  if (!ft_strncmp(sname, nmtype[i].sectionName,
						  ft_strlen(nmtype[i].sectionName)))
			  {
				  c = nmtype[i].nmType;
				  break;
			  }
			  i++;
		  }
	  }
	  if (c == '.' && !ft_strncmp(sname, ".rodata", 7))
			  c = 'r';
	  return ((c * (symbind == STB_LOCAL)) + ((c & '_') *
				  (symbind == STB_GLOBAL)));
}
