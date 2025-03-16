/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 20:45:45 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/15 21:44:09 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int	ft_strlen(char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i])
    {
        if (s1[i] != s2[i])
            return (-1);
        i++;
    }
    if (s1[i] != s2[i])
        return (-1);
    return (0);
}

int	ft_bzero(char *str, int len)
{
	int	i;

	i = -1;
	while (++i < len)
		str[i] = '\0';
    return (-1);
}
