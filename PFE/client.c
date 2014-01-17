#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include <error.h>
#include "messages.h"

void *req_socket;
void *sub_socket;

void * receiveThread(void *arg ){
	while(1){
		AS_message_t * msg = (AS_message_t *)malloc(sizeof(AS_message_t));
		zmq_recv (sub_socket, msg, sizeof(AS_message_t), 0);
    	printf ("Client SUB Received msg = %s\n", msg->name);
	}	
}

int main(int argc, char *argv[])
{
    void *context = zmq_ctx_new ();
    req_socket = zmq_socket (context, ZMQ_REQ);
    zmq_connect (req_socket, "tcp://localhost:5008");

    sub_socket = zmq_socket (context, ZMQ_SUB);
    zmq_connect (sub_socket, "tcp://localhost:5009");
    zmq_setsockopt (sub_socket, ZMQ_SUBSCRIBE,"server", 6);

    pthread_t thread;
	int rc = pthread_create( &thread, NULL, receiveThread, NULL);
	if(rc) error(EXIT_FAILURE, rc, "pthread_create");

    	char buffer [10];
        AS_message_t msg;
        msg.mess_type = 0;
		msg.client_id = 1234;
		msg.data = 1000;
		strcpy(msg.name,"Hello");

        printf ("Sending Hello msg\n");
        zmq_send (req_socket, &msg, sizeof(AS_message_t), 0);
        printf ("Client REQ Sended msg %s\n",msg.name);

        AS_message_t * msg1 = (AS_message_t *)malloc(sizeof(AS_message_t));
        zmq_recv (req_socket, msg1, sizeof(AS_message_t), 0);
        printf ("Client Received: %s %d\n", msg1->name, 0);

    pthread_join(thread, NULL);
    zmq_close (req_socket);
    zmq_ctx_destroy (context);
    return 0;
}

