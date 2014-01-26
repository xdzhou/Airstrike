/*
 * messages.c
 *
 *  Created on: 26 nov. 2011
 *      Author: cassou
 */

#ifndef MESSAGES_C_
#define MESSAGES_C_


enum messages_types {
	MSG_HELLO = 0,
	MSG_PING = 1,
	MSG_KEY = 2,
	MSG_POINTS = 3,
	MSG_NAME = 4,
	MSG_NO_SPACE = 5,
	MSG_TIME2START = 6,
	MSG_DAMAGE = 7,
	MSG_TEAM_ID = 8,
	MSG_ID_IN_TEAM = 9,
	MSG_GET_SERVER = 10,
	MSG_SET_SERVER = 11,
	MSG_DISCONNECTED = 12,
	MSG_READY = 13,
};

typedef struct
{
	int client_id;
	int mess_type;
	int data;
	char name[32];
}  __attribute__ ((__packed__)) AS_message_t;

typedef struct
{
	char name[32];
	char ipadress[32];
} Server_info_t;


struct servers_list_record
{
	Server_info_t * data;
	struct servers_list_record * next;
};

typedef struct
{
	int mess_type;
	int flag;
	Server_info_t server_info;
} Message_server_t;

#endif /* MESSAGES_C_ */
