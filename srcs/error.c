/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:53:31 by zweng             #+#    #+#             */
/*   Updated: 2022/11/23 16:56:45 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int error_msg(const char *str)
{
    ft_printf(str);
    return (FUNC_FAIL);
}

int error_msg_cleanup(const char *str, int fd, void *file)
{
    if (fd > 2)
        close(fd);
    if (file && file != MAP_FAILED)
        munmap(file);
   return (error_msg(str));
}
