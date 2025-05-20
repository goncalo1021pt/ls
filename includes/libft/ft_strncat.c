#include "libft.h"

char	*ft_strncat(char *dest, const char *src, size_t n)
{
	char	*od;
	size_t	ctd;

	ctd = 0;
	od = dest;
	dest += ft_strlen(dest);
	while (ctd < ft_strlen(src) && ctd < n)
	{
		*dest = src[ctd];
		dest++;
		ctd++;
	}
	*dest = '\0';
	return (od);
}