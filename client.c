/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnamoune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:15:54 by cnamoune          #+#    #+#             */
/*   Updated: 2024/12/19 20:53:21 by cnamoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	g_sig_get;

void	sig_get(int signal)
{
	(void)signal;
	g_sig_get = 1;
}

void	send_data(int server_pid, char *str)
{
	int				i;
	unsigned char	c;

	while (*str)
	{
		c = (unsigned char)*str;
		i = 8;
		while (i--)
		{
			if ((c >> i) & 1)
				kill(server_pid, SIGUSR2);
			else
				kill(server_pid, SIGUSR1);
			while (g_sig_get == 0)
				usleep(50);
			g_sig_get = 0;
		}
		str++;
	}
	c = '\0';
	i = 8;
	while (i--)
	{
		if(kill(server_pid, SIGUSR1) == -1)
			write(1, "Erreur d'envoi SIGUSR1\n", 24);
		while (g_sig_get == 0)
			usleep(50);
		g_sig_get = 0;
	}
}
int	main(int argc, char **argv)
{
	char	*str;
	int		server_pid;
	struct	sigaction sa;

	g_sig_get = 1;
	sa.sa_handler = sig_get;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	server_pid = ft_atoi(argv[1]);
	str = argv[2];
	if (argc != 3)
	{
		write(1, "Error ; Waiting input : Server pid Message\n", 44);
		return (1);
	}
	send_data(server_pid, str);
	return (0);
}
