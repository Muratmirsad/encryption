#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_next_line/get_next_line.h"

#define BIT_SIZE 256

static int	ft_add(const char *str, char *rstr, int irstr, int is)
{
	while (*(str + is))
	{
		*(rstr + irstr) = *(str + is);
		irstr++;
		is++;
	}
	return (irstr);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	unsigned int	is;
	unsigned int	irstr;
	char			*rstr;

	if (!s1 || !s2)
		return (0);
	is = ft_strlen(s1);
	is += ft_strlen(s2);
	rstr = (char *)malloc(is + 1);
	if (!(rstr))
		return (0);
	irstr = ft_add(s1, rstr, 0, 0);
	irstr = ft_add(s2, rstr, irstr, 0);
	*(rstr + irstr) = 0;
	return (rstr);
}

unsigned char	*create_key(int fd_key)
{
	struct timeval	current_time;
	unsigned char	*key;
	int				bit;

	bit = BIT_SIZE - 1;
	key = (unsigned char *)malloc(BIT_SIZE / 8);
	while (bit >= 0)
	{
		gettimeofday(&current_time, NULL);
		*key += (current_time.tv_usec % 2) << bit;
		printf("%d\n", current_time.tv_usec % 2);
		bit--;
	}
	bit = BIT_SIZE - 1;
	while (bit >= 0)
	{
		if (!((bit + 1) % 8))
			write (fd_key, " ", 1);
		if ((*key >> bit) % 2)
			write(fd_key, "1", 1);
		else
			write(fd_key, "0", 1);
		bit--;
	}
	return (key);
}

void	check_args(char **av, int ac, int *fd_input, int *fd_output, int *fd_key)
{
	if (ac < 2 || ac > 3)
	{
		printf("");
		//exit(2);
	}
	*fd_input = open(av[1], O_RDONLY);
	*fd_output = open(av[2], O_WRONLY | O_CREAT, 0777);
	*fd_key = open(ft_strjoin(av[2], "_key"), O_WRONLY | O_CREAT, 0777);
	if (*fd_input < 0 || *fd_output < 0)
	{
		printf("A problem was encountered while opening files.\n");
		//exit(1);
	}
}

void	helper(unsigned char *key, int fd_input, int fd_output)
{
	char	*str;
	int		c;
	int		i;
	int		j;
	
	j = 0;
	str = get_next_line(fd_input);
	while (str)
	{
		i = 0;
		while (str[i])
		{
		printf("%d\n", str[i] + key[j]);
			if (j == BIT_SIZE - 1)
				j = 0;
			if (str[i] + key[j] > 255)
				c = str[i] + key[j] - 256;
			else
				c = str[i] + key[j];
			write(fd_output, &c, 1);
			j++;
			i++;
		}
		free(str);
		str = get_next_line(fd_input);
	}
}

int	main(int ac, char **av)
{
	unsigned char	*key;
	int				fd_input;
	int				fd_output;
	int				fd_key;

	check_args(av, ac, &fd_input, &fd_output, &fd_key);
	key = create_key(fd_key);
	helper(key, fd_input, fd_output);

	close(fd_key);
	close(fd_output);
	close(fd_input);
}
