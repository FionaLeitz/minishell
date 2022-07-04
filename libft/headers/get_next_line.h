/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 10:02:14 by fleitz            #+#    #+#             */
/*   Updated: 2021/12/14 09:43:59 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# define BUFFER_SIZE 1

char	*get_next_line(int fd);
char	*ft_buffcpy(char *buff, char *str);
char	*ft_n_buffcpy(char *buff, char *str);
char	*ft_else(char *buff, char *str, size_t end);
//char	*ft_strndup(const char *s1, size_t n);
//char	*ft_strjoin(char const *s1, char const *s2);
//void	*ft_memcpy(void *dst, const void *src, size_t n);
//char	*ft_strchr(const char *s, int c);
int		ft_stop(char *buff, ssize_t rd);

#endif
