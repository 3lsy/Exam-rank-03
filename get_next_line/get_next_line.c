
#include "get_next_line.h"

int	has_enter(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

void	ft_append(char **buffer, char *helper)
{
	int	i = 0;
	while ((*buffer)[i])
		i++;
	int j = 0;
	while (helper[j])
	{
		(*buffer)[i+j] = helper[j];
		j++;
	}
	(*buffer)[i+j] = 0;
}
#include <stdio.h>
int	ft_fill_buffer(char **buffer, int fd)
{
	int	len;
	// validate BUFFER_SIZE and fd
	if (BUFFER_SIZE < 1 || fd < 0)
		return (0);
	// create a helper of size BUFFER_SIZE
	char *helper = malloc((BUFFER_SIZE + 1) * sizeof(char));
	helper[0] = 0;
	// read the fd by BUFFER_SIZE chunks
	if (*buffer == NULL)
	{
		*buffer = malloc(100000000);
		(*buffer)[0] = 0;
	}
	while (has_enter(helper) < 0)
	{
		len = read(fd, helper, BUFFER_SIZE);
		if (len <= 0)
			break ;
		helper[len] = 0;
		// printf("DBG helper: %s\n", helper);
		// "append" helper's content to buffer
		ft_append(buffer, helper);
		// printf("DBG buffer: %s\n", *buffer);
	}
	free(helper);
	if (len < 0)
	{
		free(*buffer);
		return (0);
	}
	return (1);
}

char *ft_strdup(char *s)
{
	char *new = malloc(100000);
	int i = 0;
	
	while (s[i])
	{
		new[i] = s[i];
		i++;
	}
	new[i] = 0;
	return (new);
}

void	copy_until_enter_or_EOF(char **line, char **buffer)
{
	int i = 0;
	while ((*buffer)[i] != 0 && (*buffer)[i] != '\n')
	{
		(*line)[i] = (*buffer)[i];
		i++;
		// printf("ITERATOR: %d\n", i);
	}
	(*line)[i] = (*buffer)[i];
	(*line)[i + 1] = 0;

	char *tmp = *buffer;
	if ((*buffer)[i] == '\n')
		*buffer = ft_strdup(&(*buffer)[i + 1]);
	else
		*buffer = ft_strdup("");
	free(tmp);
}

char    *get_next_line(int fd)
{
	// init static buffer
	static char	*buffer = NULL;
	// fill buffer until \n or EOF
	if (!ft_fill_buffer(&buffer, fd))
		return (NULL);
	// if the buffer is empty, free the buffer and return NULL
	if (buffer[0] == 0)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	// else "strdup until \n or EOF" from buffer to line and "trim" buffer
	char *line = malloc(100000000);
	copy_until_enter_or_EOF(&line, &buffer);
	// printf("RET: line %s\n", line);
	// printf("RET: buffer %s\n\n", buffer);
	// printf("*******************\n");
	return (line);
}

//main
#include <stdio.h>
#include <fcntl.h>

void	reading(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
}

int	main(int argc, char **argv)
{
	int		fd;

	if (argc == 1)
		fd = 0;
	else if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	else
		return (2);
	reading(fd);
	if (argc == 2)
		close(fd);
}
