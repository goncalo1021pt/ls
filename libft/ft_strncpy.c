/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: goncalo1021pt <goncalo1021pt@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 18:58:38 by gfontao-          #+#    #+#             */
/*   Updated: 2024/01/31 22:41:20 by goncalo1021      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	ctd;

	ctd = 0;
	while (src && n > 0 && src[ctd] != '\0')
	{
		dest[ctd] = src[ctd];
		ctd++;
		n--;
	}
	while (n > 0)
	{
		dest[ctd] = '\0';
		n--;
		ctd++;
	}
	return (dest);
}

/*int	main(void)
{
	char c[20], a[13] = "teste_strcpy";
	ft_strncpy(c, a, 8);
	printf("%s", c);
}*/