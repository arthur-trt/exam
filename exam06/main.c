#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////

int extract_message(char **buf, char **msg) {
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i]) {
		if ((*buf)[i] == '\n') {
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

////////////////////////////////////////////////////////////////////////////////

typedef struct	s_client {
	int				id;
	int				fd;
	char			*str;
	struct s_client	*next;
} t_client;

int			g_id		= 0;
int			g_socket	= -1;
t_client	*g_clients	= NULL;

void		del_client(t_client *client) {
	if (client)
	{
		free(client->str);
		close(client->fd);
		free(client);
	}
}

void		ft_exit(char *msg) {
	t_client	*next;

	write(2, msg, strlen(msg));
	write(2, "\n", 1);
	if (g_socket != -1)
		close(g_socket);
	while (g_clients != NULL) {
		next = g_clients->next;
		del_client(g_clients);
		g_clients = next;
	}
	exit(1);
}

t_client	**lst_last(void) {
	t_client	*tmp = g_clients;

	if (g_clients == NULL)
		return (&g_clients);
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (&tmp->next);
}


void		init_serv (int port) {
	struct sockaddr_in	serv_addr;

	if ((g_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		ft_exit("Fatal error");
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serv_addr.sin_port = htons(port);

	if ((bind(g_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))))
	{
		ft_exit("Fatal error");
	}
	if (listen(g_socket, 10) != 0)
	{
		ft_exit("Fatal error");
	}
}

void	send_one_client(t_client *client, const char *msg)
{
	int	ret;
	int	sent = 0;
	int	len = strlen(msg);

	while (sent < len) {
		if ((ret = send(client->fd, msg + sent, len - sent, MSG_DONTWAIT)) <= 0)
			return ;
		sent += ret;
	}
}

void	send_msg (const char *msg, int exception)
{
	t_client	*client = g_clients;

	while (client)
	{
		if (client->id != exception)
		{
			send_one_client(client, msg);
		}
		client = client->next;
	}
}

void	new_client (fd_set *set, int *max_fd) {
	t_client			*new_client;
	struct sockaddr_in	client;
	socklen_t			len = sizeof(client);

	if ((new_client = malloc(sizeof(t_client))) == NULL)
		ft_exit("fatal error");
	bzero(new_client, sizeof(t_client));
	*lst_last() = new_client;
	if ((new_client->fd = accept(g_socket, (struct sockaddr *)&client, &len)) < 0)
		ft_exit("fatal error");
	new_client->id = g_id++;
	{
		char	msg[50];

		if (sprintf(msg, "server: client %d just arrived\n", new_client->id) < 0)
			ft_exit("fatal error");
		send_msg(msg, new_client->id);
	}
	FD_SET(new_client->fd, set);
	if (new_client->fd > *max_fd)
		*max_fd = new_client->fd;
}

void	remove_client(t_client *to_del, fd_set *origin) {
	t_client	*curr = g_clients;

	FD_CLR(to_del->fd, origin);
	if (curr == to_del) {
		g_clients = g_clients->next;
		del_client(to_del);
		return ;
	}
	while (curr->next != to_del)
		curr = curr->next;
	curr->next = to_del->next;
	del_client(to_del);
}

void	ft_recv(t_client *client, fd_set *origin) {
	char	buff[125000 + 20 +1];
	char	*msg;
	int		ret;

	if ((ret = recv(client->fd, buff, 125000, MSG_DONTWAIT)) == 0) {
		if (sprintf(buff, "server: client %d just left\n", client->id) < 0)
			ft_exit("fatal error");
		send_msg(buff, client->id);
		remove_client(client, origin);
		return ;
	}
	else if (ret < 0)
		return ;
	buff[ret] = '\0';
	if ((client->str = str_join(client->str, buff)) == NULL)
		ft_exit("fatal error");
	while ((ret = extract_message(&client->str, &msg)) == 1) {
		if (sprintf(buff, "client %d: %s", client->id, msg) < 0)
			ft_exit("fatal error");
		send_msg(buff, client->id);
		free(msg);
	}
	if (ret == -1)
		ft_exit("fatal error");
}

void	main_loop(void) {
	fd_set	origin, copy;
	int		max_fd = g_socket;

	FD_ZERO(&origin);
	FD_SET(g_socket, &origin);
	while (1) {
		copy = origin;
		if (select(max_fd + 1, &copy, NULL, NULL, NULL) == -1)
			ft_exit("Fatal error");
		if (FD_ISSET(g_socket, &copy))
		{
			new_client(&origin, &max_fd);
		}
		{
			t_client	*next, *curr = g_clients;

			while (curr != NULL)
			{
				next = curr->next;
				if (FD_ISSET(curr->fd, &copy))
					ft_recv(curr, &origin);
				curr = next;
			}
		}
	}
}

int	main(int argc, char **argv) {
	if (argc != 2)
		ft_exit("Wrong number of argument");
	init_serv(atoi(argv[1]));
	main_loop();
}
