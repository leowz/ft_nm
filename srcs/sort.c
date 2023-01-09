/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:55:12 by zweng             #+#    #+#             */
/*   Updated: 2023/01/09 14:53:34 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int strcmp_nocase(const char *s1, const char *s2)
{
    char c1, c2;

    c1 = 0;
    c2 = 0;
    while (s1 && s2 && *s1 && *s2)
    {
       while (!ft_isalnum(*s1)) s1++;
       while (!ft_isalnum(*s2)) s2++;
       c1 = ft_tolower(*s1);
       c2 = ft_tolower(*s2);
       if (c1 == c2)
       {
           s1++;
           s2++;
       }
       else
           return (c1 - c2);
    }
    return (*s1 - *s2);
}

static int item_cmp(t_arritem *lhs, t_arritem *rhs)
{
    t_symbol    *sym1, *sym2;
    char        *name1, *name2;
    int         cmp;

    sym1 = lhs->content;
    sym2 = rhs->content;
    name1 = sym1->name;
    name2 = sym2->name;
    cmp = strcmp_nocase(name1, name2);
    if (!cmp)
    {
        if (sym2->type == 'W' && sym1->type == 'D')
            cmp = -1;
        else if (sym2->type == 'D' && sym1->type == 'W')
            cmp = 1;
        else if (sym2->type == 't' && sym1->type == 'T')
            cmp = -1;
        else if (sym2->type == 'T' && sym1->type == 't')
            cmp = 1;
        else if (sym2->type == 'T' && sym1->type == 'W')
            cmp = -1;
        else if (sym2->type == 'W' && sym1->type == 'T')
            cmp = 1;
        else
            cmp = ((sym2->type - sym1->type));
    }
    if (!cmp)
        cmp = ft_strlen(name1) - ft_strlen(name2);
    if (!cmp)
        cmp = *name1 - *name2;
    return (cmp);
}

int itemcmp_asc(t_arritem *lhs, t_arritem *rhs)
{
    return item_cmp(lhs, rhs);
}

int itemcmp_desc(t_arritem *lhs, t_arritem *rhs)
{
    return item_cmp(rhs, lhs);
}
