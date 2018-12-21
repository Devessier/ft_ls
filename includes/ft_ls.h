/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 11:10:30 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/21 11:08:44 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include <stdint.h>
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <stdbool.h>
# include <sys/types.h>
# define COLOR_RESET "\033[0m"
# define COLOR_FIFO "\033[33m"
# define COLOR_CHR "\033[34;103m"
# define COLOR_BLK "\033[34;106m"
# define COLOR_DIR "\033[36m"
# define COLOR_LNK "\033[35m"
# define COLOR_SOCK "\033[32m"
# define COLOR_EXEC "\033[31m"
# define COLOR_READ "\033[32m"
# define COLOR_WRITE "\033[38;2;255;127;80m"
# define COLOR_S "\033[38;2;216;27;96m"
# define COLOR_T "\033[38;2;255;241;118m"
# define COLOR_UID "\033[30;101m"
# define COLOR_GID "\033[30;106m"
# define COLOR_SUPER_DIR "\033[30;103m"
# define COLOR_SUPER_DIR_SAVE "\033[30;42m"
# define S_AEXEC (S_IXUSR | S_IXGRP | S_IXOTH)

typedef uint16_t	t_uflag;

typedef enum		e_flag
{
	FLAG_NONE = 0,
	FLAG_LONG_FORMAT = 1,
	FLAG_RECURSIVE = 1 << 1,
	FLAG_INCLUDE_DOTS = 1 << 2,
	FLAG_REVERSE_SORT = 1 << 3,
	FLAG_SORT_TIME_MODIFIED = 1 << 4,
	FLAG_COLORS_ON = 1 << 5,
	FLAG_NUMERIC = 1 << 6,
	FLAG_LAST_ACCESS = 1 << 7,
	FLAG_CREATION = 1 << 8,
	FLAG_STATUS_CHANGED = 1 << 9,
	FLAG_ONE_ENTRY_PER_LINE = 1 << 10,
}					t_flag;

typedef struct		s_payload
{
	struct stat	stats;
	char		*user;
	char		*group;
	char		*d_name;
	char		*d_shname;
	char		*link;
	bool		has_xattr;
	bool		has_acl;
}					t_payload;

typedef struct		s_entries
{
	t_uflag		flags;
	int			cap;
	int			len;
	t_payload	**payloads;
}					t_entries;

typedef struct		s_argument
{
	char	c_flag;
	t_flag	flag;
}					t_argument;

typedef struct		s_maxs
{
	nlink_t		links;
	size_t		links_len;
	size_t		user;
	size_t		group;
	off_t		size;
	size_t		size_len;
	dev_t		major;
	size_t		major_len;
	dev_t		minor;
	size_t		minor_len;
	blkcnt_t	blocks;
}					t_maxs;

typedef struct		s_file_type
{
	uint32_t	mode;
	char		*color;
	char		to_char;
}					t_file_type;

typedef struct		s_sort_args
{
	t_uflag	fl;
	int		start;
	int		end;
}					t_sort_args;

void				parse_args(int len, char **args);
int					append_entry(t_entries *entries, struct stat stats,
	char *long_name, char *short_name);
void				collect_entries(char **args, int len, t_uflag flags);
uint8_t				read_directory(const t_entries *entries,
	t_payload *stats, t_uflag flags, t_maxs *maximums);
void				free_stats(t_payload *stats, t_uflag flags,
	bool free_names, bool link);
void				free_entries(t_entries *entries, t_uflag flags);
uint8_t				free_grp_usr(char *grp, char *usr);
void				list_argument(t_payload *argstat, t_uflag flags);
uint8_t				error(char *path, t_uflag flags);
void				usage(char c);
void				update_maximums(t_payload *payload, t_maxs *maximums);
void				calculate_max_len(t_maxs *maximums);
void				list_file(t_payload *payload,
	t_uflag flags, t_maxs *maximums);
void				free_stats(t_payload *stats, t_uflag flags,
	bool free_names, bool link);
void				print_file_type(mode_t perms, t_uflag flags);
void				print_sticky_bit(int8_t sh, mode_t perms, t_uflag flags);
void				print_file_mode(t_payload *payload, t_uflag flags);
void				print_date(t_payload *payload, t_uflag flags);
void				print_color_file(t_payload *payload, t_uflag flags);
void				quick_sort(void **list, t_sort_args args,
	int (*f)(void*, void*, t_uflag));
int					ft_strcmp(void *s1, void *s2, t_uflag flags);
int					ft_d_name_sort(void *d1, void *s2, t_uflag flags);
int					args_sort(void *a1, void *a2, t_uflag flags);
int					time_sort(void *a1, void *a2, t_uflag flags);
void				sort_entries(void **list, int start,
	int end, t_uflag flags);
int					time_diff(void *d1, void *d2, t_uflag flags);
size_t				ft_strlen(char *str);
void				ft_putnbr_fd(int64_t n, int fd);
void				ft_putf_fd(int fd, const char *format, ...);
void				ft_putchar_color_fd(char c, char *color,
	int fd, t_uflag flags);
void				ft_putf_color_fd(int fd, char *color,
	t_uflag flags, const char *format, ...);
void				ft_putstr_fd(char *str, int fd);
void				ft_putchar_fd(char c, int fd);
char				*pathjoin(char *s1, char *s2);
size_t				nb_len(uint64_t n);
char				*ft_itoa(int64_t nb);
char				*ft_strdup(char *s1);
void				pad(int64_t c);
char				*color_code(mode_t st_mode, t_uflag flags);
void				long_format(t_payload *p, t_uflag flags, t_maxs *maximums);
time_t				set_time(const struct stat *stat, t_uflag flags);
int					time_diff(void *d1, void *d2, t_uflag flags);
char				*normalize_argument(char **str);
void				print_entry(const t_entries *entry, int total_args);
void				print(const t_entries *files_args,
	const t_entries *dir_args, int len, t_maxs *f_maxs);
void				sort_entries(void **list, int start,
	int end, t_uflag flags);

extern t_file_type	g_file_types[];
extern t_argument	g_arguments[];

#endif
