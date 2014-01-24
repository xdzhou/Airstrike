/*  =====================================================================
    zhelpers.h

    Helper header file for example applications.
    =====================================================================
*/

#ifndef __ZMQHELPER_H__
#define __ZMQHELPER_H__

//  Include a bunch of headers that we will need in the examples

#include <zmq.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <malloc.h>
#include "messages.h"

//  Version checking, and patch up missing constants to match 2.1
#if ZMQ_VERSION_MAJOR == 2
#   error "Please upgrade to ZeroMQ/3.2 for these examples"
#endif

//  Provide random number from 0..(num-1)
#if (defined (__WINDOWS__))
#   define randof(num)  (int) ((float) (num) * rand () / (RAND_MAX + 1.0))
#else
#   define randof(num)  (int) ((float) (num) * random () / (RAND_MAX + 1.0))
#endif


//  Receive 0MQ string from socket and convert into C string
//  Caller must free returned string. Returns NULL if the context

//  send and receive a msg
static AS_message_t * s_recv_msg (void *socket) {
    AS_message_t * msg = (AS_message_t *)malloc(sizeof(AS_message_t));
    int size = zmq_recv (socket, msg, sizeof(AS_message_t), 0);
    if (size == -1)return NULL;
    return msg;
}
static int s_send_msg (void *socket, AS_message_t * msg) {
    int size = zmq_send (socket, msg, sizeof(AS_message_t), 0);
    return size;
}
//  send and receive a server_info msg 
static Message_server_t * s_recv_server_msg (void *socket) {
    Message_server_t * msg = (Message_server_t *)malloc(sizeof(Message_server_t));
    int size = zmq_recv (socket, msg, sizeof(Message_server_t), 0);
    if (size == -1)return NULL;
    return msg;
}
static int s_send_server_msg (void *socket, Message_server_t * msg) {
    int size = zmq_send (socket, msg, sizeof(Message_server_t), 0);
    return size;
}
//send and receive a string
static char * s_recv_str (void *socket) {
    char buffer [256];
    int size = zmq_recv (socket, buffer, 255, 0);
    if (size == -1) return NULL;
    if (size > 255) size = 255;
    buffer [size] = 0;
    return strdup (buffer);
}
static int s_send_str (void *socket, char *string) {
    int size = zmq_send (socket, string, strlen (string), 0);
    return size;
}
//  Send and receive a msg (publiser)
static int s_send_pub (void *socket, AS_message_t * msg, int clientid) {
    int size = zmq_send (socket, &clientid, sizeof(int), ZMQ_SNDMORE);
    size = zmq_send (socket, msg, sizeof(AS_message_t), 0);
    return size;
}
static AS_message_t * s_recv_sub (void *socket) {
    int *clientid = (int *)malloc(sizeof(int));
    zmq_recv (socket, clientid, sizeof(int), 0);
    AS_message_t * msg = (AS_message_t *)malloc(sizeof(AS_message_t));
    zmq_recv (socket, msg, sizeof(AS_message_t), 0);
    free(clientid);
    return msg;
}

//  Set simple random printable identity on socket
//
static void s_set_id (void *socket)
{
    char identity [10];
    sprintf (identity, "%04X-%04X", randof (0x10000), randof (0x10000));
    zmq_setsockopt (socket, ZMQ_IDENTITY, identity, strlen (identity));
}


//  Sleep for a number of milliseconds
static void s_sleep (int msecs)
{
#if (defined (__WINDOWS__))
    Sleep (msecs);
#else
    struct timespec t;
    t.tv_sec  =  msecs / 1000;
    t.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep (&t, NULL);
#endif
}

//  Return current system clock as milliseconds
static int64_t s_clock (void)
{
#if (defined (__WINDOWS__))
    SYSTEMTIME st;
    GetSystemTime (&st);
    return (int64_t) st.wSecond * 1000 + st.wMilliseconds;
#else
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (int64_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

//  Print formatted string to stdout, prefixed by date/time and
//  terminated with a newline.

static void s_console (const char *format, ...)
{
    time_t curtime = time (NULL);
    struct tm *loctime = localtime (&curtime);
    char *formatted = (char *)malloc (20);
    strftime (formatted, 20, "%y-%m-%d %H:%M:%S ", loctime);
    printf ("%s", formatted);
    free (formatted);

    va_list argptr;
    va_start (argptr, format);
    vprintf (format, argptr);
    va_end (argptr);
    printf ("\n");
}

#endif  //  __ZMQHELPER_H__
