#include "memcached.h"
#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *_create_struct_request(char *command, mm_data_info info);
char *_create_str_request(char *command, char *str);

void _append_ending(char **str);

void _send_mm_req(int fd, char *req);
char *_recv_mm_resp(int fd);

void memcached_init(struct memcached *m)
{
    m->addr = "127.0.0.1";
    m->port = 11211;
    _debug_print("Trying to connect to %s:%d...\n", m->addr, m->port);

    m->fd = socket(AF_INET, SOCK_STREAM, 0);
    struct in_addr s_addr;
    inet_pton(AF_INET, m->addr, &s_addr.s_addr);

    struct sockaddr_in addr;
    addr.sin_addr = s_addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m->port);
    connect(m->fd, (const struct sockaddr *)&addr, sizeof(addr));
    _debug_print("Connection Succesful!\n");
}

int memcached_add(struct memcached *m, struct mm_data_info info, char *value)
{
    char *req = _create_struct_request("add", info);
    char *val = strdup(value);

    _append_ending(&req);
    _append_ending(&val);

    _send_mm_req(m->fd, req);
    _send_mm_req(m->fd, val);

    char *res = _recv_mm_resp(m->fd);
    // TODO error Handling (enum)
    return 0;
}

char *memcached_get(struct memcached *m, char *key)
{
    char *req = _create_str_request("get", key);
    _append_ending(&req);

    _send_mm_req(m->fd, req);
    //TODO error Handling
    char *res = _recv_mm_resp(m->fd);
}

void memcached_exit(struct memcached *m)
{
    _debug_print("Closing Connection...\n");
    close(m->fd);
}

void _send_mm_req(int fd, char *req)
{
    // TODO iterate with while cycle
    write(fd, req, strlen(req));
    _debug_print("Request Sent : %s", req);
}

char *_recv_mm_resp(int fd)
{
    int buff_size = 100;
    char *buffer = malloc(sizeof(char) * buff_size);
    // TODO iterate with while
    int t = read(fd, buffer, buff_size);
    buffer[t] = '\0';
    _debug_print("Response Recieved : %s", buffer);
    return buffer;
}

char *_create_str_request(char *command, char *str)
{
    int to_alloc = strlen(command) + strlen(str) + 1;
    char *req = malloc(to_alloc);

    strcat(req, command);
    strcat(req, " ");
    strcat(req, str);

    return req;
}

char *_create_struct_request(char *command, mm_data_info info)
{
    char flags_str[50];
    sprintf(flags_str, "%d", info.flags);

    char ttl_str[50];
    sprintf(ttl_str, "%d", info.ttl);

    char size_str[50];
    sprintf(size_str, "%d", info.size);

    int to_alloc = strlen(command) + strlen(info.key) + strlen(flags_str) + strlen(ttl_str) + strlen(size_str) + 7;
    char *req = malloc(to_alloc);

    strcat(req, command);
    strcat(req, " ");

    strcat(req, info.key);
    strcat(req, " ");

    strcat(req, flags_str);
    strcat(req, " ");

    strcat(req, ttl_str);
    strcat(req, " ");

    strcat(req, size_str);
    return req;
}

void _append_ending(char **str)
{
    *str = realloc(*str, strlen(*str) + 2);
    strcat(*str, "\r\n");
}