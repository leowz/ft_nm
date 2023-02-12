/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:55:12 by zweng             #+#    #+#             */
/*   Updated: 2023/02/10 17:38:12 by vagrant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int strcmp_case(const char *s1, const char *s2, int case_sensitive)
{
    char c1, c2;

    c1 = 0;
    c2 = 0;
    while (s1 && s2 && *s1 && *s2)
    {
       while (!ft_isalnum(*s1)) s1++;
       while (!ft_isalnum(*s2)) s2++;
       if (case_sensitive == FALSE)
       {
           c1 = ft_tolower(*s1);
           c2 = ft_tolower(*s2);
       }
       if (c1 == c2)
       {
           s1++;
           s2++;
       }
       else
           return (c2 - c1);
    }
    return (*s2 - *s1);
}

static int item_cmp(t_arritem *lhs, t_arritem *rhs)
{
    t_symbol    *sym1, *sym2;
    char        *name1, *name2;
    int         cmp;
    uint64_t    value1, value2;

    sym1 = lhs->content;
    sym2 = rhs->content;
    name1 = sym1->name;
    name2 = sym2->name;
    value1 = sym1->value;
    value2 = sym2->value;
    cmp = strcmp_case(name2, name1, FALSE);
    if (!cmp)
        cmp = value1 - value2;
    /*if (!cmp)
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
    }*/
    /*if (!cmp)
        cmp = strcmp_case(name2, name1, TRUE);*/
    if (!cmp)
        cmp = ft_strcmp(name2, name1);
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
