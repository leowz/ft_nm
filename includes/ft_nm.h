/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 17:08:48 by zweng             #+#    #+#             */
/*   Updated: 2022/11/27 19:13:37 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include "libft.h"
# include "ft_printf.h"
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <ar.h>
# include <elf.h>

# include <stdio.h>

typedef enum    e_param {
    ARG_A = 1 << 0,
    ARG_G = 1 << 1,
    ARG_U = 1 << 2,
    ARG_R = 1 << 3,
    ARG_P = 1 << 4,
}               t_param;

/*
 *
 */
int             ft_nm32(const void *file, size_t size, t_param params);
int             ft_nm64(const void *file, size_t size, t_param params);

/*
 *
 * helper functions
 */
int             check_elf_ident(char *file, unsigned char *arch, size_t size);
int             is_special_section_indice(uint16_t s_idx);
int             error_msg(const char *str);
int             error_msg_cleanup(const char *str, int fd,
                    void *file, size_t filesize);
#endif

