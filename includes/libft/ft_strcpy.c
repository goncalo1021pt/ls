/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: goncalo1021pt <goncalo1021pt@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:27:57 by gfontao-          #+#    #+#             */
/*   Updated: 2024/01/31 22:39:58 by goncalo1021      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char	*ft_strcpy(char *dest, char *src)
{
	unsigned int	ctd;

	ctd = 0;
	while (src && src[ctd] != '\0')
	{
		dest[ctd] = src[ctd];
		ctd++;
	}
	dest[ctd] = '\0';
	return (dest);
}

/*int main (){

char	c[20],a[13] = "teste_strcpy";
	ft_strcpy(c,a);
	printf("%s",c);
}*/