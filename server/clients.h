#ifndef __CLIENTS__H
#define __CLIENTS__H

#include "config.h"

extern client_s clients[MAX_CLIENTS];

void prep_client_data();

void send_to_all_clients(char* message, int length);

void release_client(client_s* client, char* str_message);

#endif // __CLIENTS__H
