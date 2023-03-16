#include<stdio.h>
#include<string.h>
#include<stdint.h>

#include "commands.h"
#include "clients.h"
#include "message_queue.h"

typedef struct {
    char* name;
    void (*cmd)(client_s*,char*,int read_len);
} command_s;

uint64_t get_dist_to_whitespace(char* str, char* whitespace,int search_len){
    uint64_t dist = search_len;
    uint64_t cur_dist = 0;

    for(uint64_t i=0; i < strlen(whitespace);i++){
        char* trimed_str = strchr(str,whitespace[i]);

        if(trimed_str == NULL)
            continue;

        cur_dist = trimed_str - str;

        if(cur_dist < dist)
            dist = cur_dist;
    }

    return dist;
}

/*
** FIXME:
** if the nick name is longer than like 10 chars breaks stuff somehow.
** only on some systems, like aws debian server.
*/

void cmd_nick(client_s* client, char* command,int read_len){
    command+=5;
    read_len-=5;

    int len = get_dist_to_whitespace(command,"\n\t\r",read_len); //uh oh.

    if(len==0){
        send_message(client,"you must provide a name to change to\n",NULL);
        return;
    }

    if(len > 15)
        len=15;

    for(int i=0;i<MAX_CLIENTS;i++){
        if(memcmp(clients[i].name,command,15) == 0){
            send_message(client,"That name is already taken\n",NULL);
            return;
        }
    }


    char old_name[16];
    memcpy(old_name,client->name,16);

    memset(client->name,0,16);
    memcpy(client->name,command,len);


    //TODO FIND A BETTER WAY TO DO THIS.
    pthread_mutex_t msg_lock;
    pthread_mutex_init(&msg_lock,NULL);
    pthread_mutex_lock(&msg_lock);

    char message[256] = {0};
    sprintf(message,"%s Has Changed Their Name To: %s\n",old_name,client->name);
    broadcast_message(message,&msg_lock);

    //waits for the mutex to be unlocked by the message_queue thread.
    pthread_mutex_lock(&msg_lock);
    //mutex must be unlocked before being destroyed (see man pthread_mutex_destroy)
    pthread_mutex_unlock(&msg_lock);
    pthread_mutex_destroy(&msg_lock);
}

void cmd_test(client_s* client, char* command, int read_len){
    send_message(client,"Test Command Executed\n",NULL);
}

//fix this if it does not work.
const command_s commands[] ={
    { .name = "nick", .cmd = &cmd_nick},
    { .name = "test", .cmd = &cmd_test}
};
#define CMD_LEN sizeof(commands)/sizeof(command_s)

void process_command(client_s* client, char* command, int read_len){
    //ignore the frist char (/)
    command++;
    read_len--;

    uint64_t cmd_len = get_dist_to_whitespace(command,"\t\n\r ",read_len);

    for(int i=0;i<CMD_LEN;i++)
        if(strlen(commands[i].name) == cmd_len)
            if(memcmp(commands[i].name,command,cmd_len) == 0)
                (commands[i].cmd)(client,command,read_len);
}
