/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnamoune <cnamoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:49:34 by cnamoune          #+#    #+#             */
/*   Updated: 2024/12/20 00:10:24 by cnamoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void    get_signal(int signum, siginfo_t *info, void *context)
{
    static int             bit_nb;
    static unsigned char   signal = 0;
    static char           *buffer;

	(void)context;
    if (signum == SIGUSR2)
        signal |= (1 << (7 - bit_nb));
    bit_nb++;
    if (bit_nb == 8)
    {
        if (signal == '\0')
        {
            if (buffer)
            {
                write(1, buffer, ft_strlen(buffer));
                write(1, "\n", 1);
                free(buffer);
                buffer = NULL;
            }
        }
        else
            buffer = ft_strjoin(buffer, signal);
        signal = 0;
        bit_nb = 0;
    }
    if (info && info->si_pid > 0)
        kill(info->si_pid, SIGUSR1);
}
int	main(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	ft_putnbr(getpid());
	write(1, "\n", 1);

	sa.sa_sigaction = get_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;

	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		write(1, "Error with SIGUSR1. Closing server now.", 40);
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		write(1, "Error with SIGUSR1. Closing server now.", 40);
		exit(EXIT_FAILURE);
	}
	while (1)
		pause();
	return (0);
}
