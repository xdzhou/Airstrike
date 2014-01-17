#include <zmq.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <pthread.h>
#include <error.h>
#include "messages.h"

void *rep_socket;
void *pub_socket;
void *pull_socket;

void * sendThread(void *arg ){
	while(1){
		sleep (4);
		AS_message_t msg;
	    msg.mess_type = 0;
		msg.client_id = 1234;
		msg.data = 1000;
		strcpy(msg.name,"server send msg");
		zmq_send (pub_socket, &msg, sizeof(AS_message_t), ZMQ_SNDMORE);
		printf ("server PUB sed msg = %s\n", msg.name);
	}
	
}

int main(int argc, char *argv[])
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    rep_socket = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (rep_socket, "tcp://*:5008");
    assert (rc == 0);

    pub_socket = zmq_socket (context, ZMQ_PUB);
    rc = zmq_bind (pub_socket, "tcp://*:5009");
    assert (rc == 0);

    pull_socket = zmq_socket (context, ZMQ_PULL);
    rc = zmq_bind (pull_socket, "tcp://*:5010");
    assert (rc == 0);

    //msg
    AS_message_t * msg = (AS_message_t *)malloc(sizeof(AS_message_t));
    printf ("Ready go\n");

    pthread_t thread;
	rc = pthread_create( &thread, NULL, sendThread, NULL);
	if(rc) error(EXIT_FAILURE, rc, "pthread_create");

    while(1){
    	rc = zmq_recv (rep_socket, msg, sizeof(AS_message_t), 0);
    	printf ("server REP Received msg = %s\n", msg->name);

    	strcpy(msg->name,"server feedback");
    	zmq_send (rep_socket, msg, sizeof(AS_message_t), 0);
        printf ("server REP feedback\n");

    }
    

    return 0;
}

