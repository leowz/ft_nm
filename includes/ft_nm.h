/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zweng <zweng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 17:08:48 by zweng             #+#    #+#             */
/*   Updated: 2023/02/14 13:43:01 by zweng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include "libft.h"
# include "ft_printf.h"
# include "ft_array.h"
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <ar.h>
# include <elf.h>

# define HAS_ARG(x, y) ((x & y) > 0)

typedef enum    e_param {
    ARG_A = 1 << 0,
    ARG_G = 1 << 1,
    ARG_U = 1 << 2,
    ARG_R = 1 << 3,
    ARG_P = 1 << 4,
}               t_param;

typedef struct  s_symbol{
    unsigned char   type;
    char            *name;
    uint64_t        size;
    uint64_t        value;
    uint16_t        index;
    void            *symptr;
}               t_symbol;
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
void            set_sym_arr(t_symbol *arr, unsigned int index,
                unsigned int value, unsigned char type, char *name);
int             itemcmp_desc(t_arritem *lhs, t_arritem *rhs);
int             itemcmp_asc(t_arritem *lhs, t_arritem *rhs);
int             itemcmp_value_asc(t_arritem *lhs, t_arritem *rhs);
void            delete_array(t_array **arr);
unsigned int	get_sym_type64(Elf64_Ehdr *ehdr, Elf64_Shdr *shdrt,
                Elf64_Sym cur_sym);
unsigned int	get_sym_type32(Elf32_Ehdr *ehdr, Elf32_Shdr *shdrt,
                Elf32_Sym cur_sym);
unsigned char   type_adjust(const char *sname, char type, unsigned int bind);
/*
 *
 * endian functions
 *
 */ 
int             need_change_endian(char *file);
void            reverse_endian(void *ptr, size_t length);
uint16_t	    read_uint16(uint16_t nb);
uint32_t        read_uint32(uint32_t nb);
uint64_t        read_uint64(uint64_t nb);
unsigned int	read_unsigned_int(unsigned int nb);
long unsigned int   read_long_unsigned_int(long unsigned int nb);
int			    read_int(int nb);

#endif

