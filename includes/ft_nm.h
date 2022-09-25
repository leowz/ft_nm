#ifndef FT_NM_H
# define FT_NM_H

# include "libft.h"
# include "ft_printf.h"
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <ar.h>
# include <elf.h>


# ifdef __i386__
#  define GET_ARCH "i386"
# endif

# ifdef __x86_64__
#  define GET_ARCH "x86_64"
# endif

# ifdef __ppc__
#  define GET_ARCH "ppc"
# endif

# ifndef __i386__
#  ifndef __ppc__
#   ifndef __x86_64__
#    define GET_ARCH ""
#   endif
#  endif
# endif

# define ERR_INVALID "Error: Invalid"
# define ERR_MALFORMED "Error: Malformed"
# define ERR_UNDEFINED "Error: Undefined"
# define ERR_MALLOC "Error: Malloc Failed"

# define ERR_OPEN_FMT "{red}%s{no}: '%s': open() fail\n"
# define ERR_FSTAT_FMT "{red}%s{no}: fd:'%d' : fstat() fail\n"
# define ERR_IS_DIR_FMT "{red}%s{no}: fd:'%d': it is a directory \n"
# define ERR_MMAP_FMT "{red}%s{no}: fd:'%d' : mmap() fail\n"
# define ERR_INVALID_FMT "{red}%s{no}: The file %s is not valid object file\n"
# define ERR_MALFORMED_FMT "{red}%s{no}: %s truncated or malformed object\n"
# define ERR_MALLOC_FMT "{red}%s{no}: Malloc return Null in %s\n"

int						    print_error(const char *pgm, int ret);
int							return_error(const char *pgm, int ret);
void						*return_error_null(char *pgm, int ret);

# define MALLOC_LVL_FILE_MACH_O M_LVL_OFFSET + 1

typedef enum				e_endian {
	BIG,
	LITTLE,
	UNKNOWN
}							t_endian;

# ifdef __BIG_ENDIAN__
#  define GET_ENDIAN (t_endian)(BIG)
# endif

# ifdef __LITTLE_ENDIAN__
#  define GET_ENDIAN (t_endian)(LITTLE)
# endif

# define AR_LONG_NAME sizeof(char[20])
# define SIZEOF_OBJ_H  sizeof(struct ar_hdr) + AR_LONG_NAME
# define AR_LEN_BEFORE_SYMTAB SARMAG + SIZEOF_OBJ_H
# define t_bool     int
# define PROGRAM "ft_nm"

typedef enum				e_type_macho {
	M_32,
	M_64,
	M_END,
	M_ERR
}							t_type_macho;

typedef struct				s_arch {
	const char				*path;
	const void				*data;
	size_t					length;
    unsigned char           *ident;
	t_endian				endian_type;
    t_type_macho            macho_type;
}							t_arch;

int						    map_file(const char *file, t_arch *arch);
void						*secure_add(const t_arch input,
										const void *start_add, size_t length);
void						*secure_add_mv(t_arch input, const void *start_add,
											size_t length);
char						*secure_string(t_arch input, char *string_table,
											uint32_t offset);
uint32_t					get_magic(t_arch *arch);
int							exec_handler(t_arch *arch);
t_type_macho				which_header(t_arch *arch);
int						    handler_64(t_arch *input, Elf64_Ehdr *header);
int						    handler_32(t_arch *input, Elf32_Ehdr *header);

/*uint16_t					ft_swap_int16(uint16_t x);
uint32_t					ft_swap_int32(uint32_t x);
uint64_t					ft_swap_int64(uint64_t x);

uint16_t					ifswap16(t_arch *input, uint16_t x);
uint32_t					ifswap32(t_arch *input, uint32_t x);
uint64_t					ifswap64(t_arch *input, uint64_t x);

void						swap_fat_header(struct fat_header *fh);
void						swap_mach_header_32(struct mach_header *mh);
void						swap_mach_header_64(struct mach_header_64 *mh);
void						swap_symtab_command(struct symtab_command *st);
void						swap_section_64(struct section_64 *s,
											uint32_t nsects);
void						swap_section(struct section *s, uint32_t nsects);
void						swap_nlist_64(struct nlist_64 *symbol);
void						swap_nlist(struct nlist *symbol);
*/

/*int							get_header_32(const t_arch arch, const void *add,
											struct mach_header *header);
int							get_header_64(const t_arch arch, const void *add,
											struct mach_header_64 *header);
int							get_header_fat(const t_arch arch, const void *add,
											struct fat_header *header);
int							get_header_lib(const t_arch arch, const void *add,
											struct ar_hdr *header);
                                            */
/*typedef struct				s_arch_info
{
	const char				*name;
	cpu_type_t				cputype;
	cpu_subtype_t			cpusubtype;
	t_endian				byte_order;
}							t_arch_info;

t_arch_info					get_arch_info_from_cpu_type(cpu_type_t cputype,
											cpu_subtype_t cpusubtype);


typedef enum			e_type_arch {
	ARCH_32,
	ARCH_64
}						t_type_arch;

typedef struct			s_sym {
	char				*name;
	char				sectname[16];
	char				segname[16];
	uint64_t			value;
	uint8_t				ntype;
	uint8_t				nsect;
	t_type_arch			arch;
}						t_sym;

typedef struct			s_nm_flags {
	t_bool				a;
	t_bool				g;
	t_bool				a_up;
	t_bool				n;
	t_bool				p;
	t_bool				r;
	t_bool				u;
	t_bool				u_up;
	t_bool				m;
	t_bool				j;
	t_bool				print_arch;
	t_handler_func		*funcs;
}						t_nm_flags;


t_nm_flags				*get_nm_flags();
int						intern_nm_parse_option(int ac, const char **av);

char					get_char_type(t_sym sym);
void					print_value(t_sym sym, char c);

int						handler_64(t_arch *input);
int						handler_32(t_arch *input);
int						handle_fat(t_arch *input);
int						handle_lib(t_arch *input);
char					get_sim_type(struct nlist_64 sym, t_arch input);

int						find_seg_sect_name_64(struct nlist_64 symbol,
												t_sym *elem_sym, t_arch input);
int						find_seg_sect_name(struct nlist symbol, t_sym *elem_sym,
																t_arch input);

struct symtab_command	*get_symtab_lc(t_arch input);

void					print_nm(t_arch *input);
void					print_m(t_sym sym);
char					get_char_type(t_sym sym);

void					sort_sym(t_list **lst_sym);
int 					handler_32(t_arch *input);
int 					handler_64(t_arch *input);
int 					handler_lib(t_arch *input);
int 					handler_fat(t_arch *input);
*/
#endif

