/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:53:31 by zweng             #+#    #+#             */
/*   Updated: 2022/11/27 17:19:42 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int error_msg(const char *str)
{
    ft_printf(str);
    return (FUN_FAIL);
}

int error_msg_cleanup(const char *str, int fd, void *file, size_t filesize)
{
    if (fd > 2)
        close(fd);
    if (file && file != MAP_FAILED)
        munmap(file, filesize);
   return (error_msg(str));
}
