/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:57:09 by zweng             #+#    #+#             */
/*   Updated: 2022/11/27 18:55:36 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int check_elf_ident(char *file, unsigned char *arch, size_t size)
{
    if (size < 16)
        return (FUN_FAIL);
    *arch = file[EI_CLASS];
    ft_printf("%d, %d, %d, %d\n", !ft_memcmp(file, ELFMAG, SELFMAG), file[EI_CLASS], file[EI_DATA], file[EI_VERSION]);
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
