/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 18:59:28 by zweng             #+#    #+#             */
/*   Updated: 2022/12/11 19:34:30 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"


void    add_to_list(t_array **arr, unsigned int val, char t, char *name)
{
    t_symbol *sym;
    
    if (!*arr && (*arr = ft_arrnew()))
        return ;
    if ((sym = malloc(sizeof(*sym))))
    {
       sym->value = val;
       sym->type = t;
       sym->name = name;
       ft_arrappend_raw(*arr, (void *)sym, sizeof(*sym));
    }
}
