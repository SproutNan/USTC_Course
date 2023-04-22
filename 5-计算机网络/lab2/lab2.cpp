#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <queue>
#include <map>

#include "lab2.h"

Client clients_list[MAX_CLIENT_NUMBER];
std::map<int, int> client_map; // key: fd, value: index
int connected_clients_num = 0;
timev_t timeout = {0, 500};

void epoll_add(int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}

void display_client()
{
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        putchar(clients_list[i].first ? '1' : '0');
    }
    putchar('\n');
}

void client_destroy(int epfd, Client *user)
{
    epoll_ctl(epfd, EPOLL_CTL_DEL, user->second, 0);
    user->first = false;
    close(user->second);
    connected_clients_num--;
    client_map.erase(user->second);
    printf("[QUIT] one client left, now %d in total\n", connected_clients_num);
}

int client_add(int epfd, int fd)
{
    int index = -1;
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        if (!clients_list[i].first)
        {
            index = i;
            break;
        }
    }
    if (index < 0)
    {
        return -1;
    }
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
    clients_list[index].first = true;
    clients_list[index].second = fd;
    connected_clients_num++;

    client_map[fd] = index;

    epoll_add(epfd, fd);

    printf("[ENTER] new client added, now %d in total\n", connected_clients_num);

    return index;
}

void send_all(Client *client, std::string s)
{
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        if (clients_list[i].first && &clients_list[i] != client)
        {
            int send_len = 1;
            auto mess = s;
            do
            {
                int send_num = send(clients_list[i].second, mess.data(), mess.length(), 0); // 本次发送的字符数
                mess = mess.substr(send_num);                                               // 假如本次发了4个字符，就要从原来字符串的第4个字符重新开始发
                send_len = mess.length();                                                   // 需要发送的字符数
            } while (send_len);
        }
    }
}

ssize_t receive(Client *client, void *buf, size_t n)
{
    int size = recv(client->second, buf, n, 0);
    return size;
}

int main(int argc, char **argv)
{
    int port = atoi(argv[1]);
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket");
        return 1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    socklen_t addr_len = sizeof(addr);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind");
        return 1;
    }
    if (listen(fd, 2))
    {
        perror("listen");
        return 1;
    }
    // 初始化epoll
    int epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0)
    {
        perror("epoll_create");
        return 1;
    }
    ep_evt events[EPOLL_SIZE];
    epoll_add(epfd, fd);

    // 初始化所有客户端
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        clients_list[i].first = false;
    }

    while (true)
    {
        int ep_evts_num = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        for (int i = 0; i < ep_evts_num; i++)
        {
            if (events[i].data.fd == fd)
            {
                // 新连接
                int new_fd = accept(fd, NULL, NULL);
                if (new_fd >= 0)
                {
                    client_add(epfd, new_fd);
                }
                else
                    continue;
            }
            else
            {
                int new_fd = events[i].data.fd;
                int index;
                try
                {
                    index = client_map.at(new_fd);
                }
                catch (...)
                {
                    printf("map error\n");
                    return 1;
                }
                if (clients_list[index].first)
                {
                    bool flag = true;
                    while (true)
                    {
                        char buffer[MAX_SINGLE_MESSAGE_LENGTH] = "";
                        auto len = receive(&clients_list[index], buffer, MAX_SINGLE_MESSAGE_LENGTH - 1);

                        if (len <= 0)
                        {
                            if (flag)
                            {
                                client_destroy(epfd, &clients_list[index]);
                            }
                            break;
                        }

                        flag = false;

                        auto mess_box = split(buffer, "\n");
                        auto messes = mess_box.first;
                        auto clip_num = mess_box.second;

                        for (int j = 0; j < clip_num; j++)
                        {
                            if (messes[j].length())
                            {
                                if (*(messes[j].rbegin()) != '\0')
                                {
                                    messes[j] += '\0';
                                }
                                std::string mess = " [Message]";
                                mess += messes[j];
                                mess += "\n";
                                send_all(&clients_list[index], mess);
                            }
                        }
                    }
                    fcntl(clients_list[index].second, F_SETFL, fcntl(clients_list[index].second, F_GETFL, 0) | O_NONBLOCK);
                }
            }
        }
    }
    // close(epfd);
    return 0;
}
