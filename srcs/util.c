/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:57:09 by zweng             #+#    #+#             */
/*   Updated: 2022/11/23 17:24:33 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int check_elf_ident(void *file, unsigned char *arch, size_t size)
{
    if (size < 16)
        return (FUNC_FAIL);
    *arch = file[EI_CLASS];
    if (!ft_memcpy(file, ELFMAG, SELFMAG) || 
            file[EI_CLASS] <= ELFCLASSNONE || file[EI_CLASS] > ELFCLASS64 ||
            file[EI_DATA] <= ELFDATANONE || file[EI_DATA] > ELFDATA2MSB ||
            file[EI_VERSION] != EV_CURRENT)
        return (FUNC_FAIL);
    else
        return (FUNC_SUCS);
}
