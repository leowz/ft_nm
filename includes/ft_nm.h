/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 17:08:48 by zweng             #+#    #+#             */
/*   Updated: 2022/11/13 17:53:48 by zweng            ###   ########.fr       */
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

typedef enum    e_param {
    ARG_A = 1 << 0,
    ARG_G = 1 << 1,
    ARG_U = 1 << 2,
    ARG_R = 1 << 3,
    ARG_P = 1 << 4,
}               t_param;


#endif

