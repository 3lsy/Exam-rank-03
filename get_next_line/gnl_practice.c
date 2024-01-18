/*
#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <unistd.h>
#include <stdlib.h>

# ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
# endif

#endif
*/

#include "get_next_line.h"

char *ft_strdup(char *s){
	char *new = malloc(1000000 * sizeof(char));
	int i = 0;
	while (s[i]){
		new[i] = s[i];
		i++;
	}
	new[i] = 0;
	return (new);
}

int has_enter(char *s){
	int i = 0;
	while (s[i]){
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

void append_helper(char *helper, char **buffer)
{
	int i = 0;
	int j = 0;
	while((*buffer)[i]){
		i++;
	}
	while (helper[j]){
		(*buffer)[i+j] = helper[j];
		j++;
	}
	(*buffer)[i+j] = 0;
}

int ft_fill_buffer(int fd, char **buffer)
{
	int len;

	char *helper = malloc((BUFFER_SIZE + 1) * sizeof(char));
	helper[0] = 0;
	if (!helper || BUFFER_SIZE < 1 || fd < 0){
		free(helper);
		return (0);
	}
	if (*buffer == NULL){
		*buffer = malloc(100000000 * sizeof(char));
		(*buffer)[0] = 0;
	}
	while (has_enter(helper) < 0){
		len = read(fd, helper, BUFFER_SIZE);
		if (len <= 0)
			break;
		helper[len] = 0;
		append_helper(helper, buffer);
	}
	free(helper);
	if (len < 0)
		return (0);
	return (1);
}

void copy_until_enter_or_EOF(char **line, char **buffer)
{
	int i = 0;
	while ((*buffer)[i] != 0 && (*buffer)[i] != '\n'){
		(*line)[i] = (*buffer)[i];
		i++;
	}
	(*line)[i] = (*buffer)[i];
	(*line)[i+1] = 0;

	char *tmp = *buffer;
	if ((*buffer)[i] == '\n'){
		*buffer = ft_strdup(&(*buffer)[i+1]);
	}
	else{
		*buffer = ft_strdup("");
	}
	free(tmp);
}

char    *get_next_line(int fd)
{
	static char *buffer = NULL;
	if (!ft_fill_buffer(fd, &buffer))
		return (NULL);
	if (buffer[0] == 0){
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	char *line = malloc(100000000 * sizeof(char));
	copy_until_enter_or_EOF(&line, &buffer);
	return (line);
}

//main
#include <stdio.h>
#include <fcntl.h>

void reading(int fd){
	char *line = get_next_line(fd);
	while(line){
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	if (line){
		free(line);
	}
}

int main(int ac, char **av)
{
	int fd = 0;
	if (ac == 1){
		fd = 0;
	}
	else if (ac == 2){
		fd = open(av[1], O_RDONLY);
	}
	else
		return 2;
	reading(fd);
	if (ac == 2){
		close(fd);
	}
}