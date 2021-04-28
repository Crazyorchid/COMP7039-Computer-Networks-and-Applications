#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "emulator.h"
#include "gbn.h"
#include <string.h>

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

#define BIDIRECTIONAL 0   /* change to 1 for bidirectional */
                           /* and write a routine called B_output */
/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
/*
Global variables
*/
int MAX_BUFFER = 1500;
#define WINDOW_SIZE 6
struct pkt queue[50000];
struct pkt receiver_buffer[WINDOW_SIZE];
int seqnums_A;
int base_A;

struct pkt data_packet_A;

int last_received_B;
float timeout_value;
int from_app_A=0;
int from_transport_A=0;
int to_transport_B=0;
int to_app_B=0;


int get_checksum(int seqnum, int acknum, char payload[20])
{
  int checksum = 0;
  checksum += seqnum + acknum;
  if (payload != NULL) {
    int i;
    for ( i=0; i < 20; i++)
    {
      checksum += (int)payload[i];
    }
  }
  return checksum;
}

struct pkt create_pkt(int seqnum, int acknum, char payload[20]) {
  struct pkt packet;
  packet.seqnum = seqnum;
  packet.acknum = acknum;
  if (payload != NULL) {
    strncpy(packet.payload, payload, 20);
  }
  packet.checksum = get_checksum(seqnum, acknum, packet.payload);
  return packet;
}

/* void log_packet(char calling_entity, struct pkt packet) {
  printf("packet sent from %c with message %.*s and seq number %d\n", calling_entity, (int)sizeof(packet.payload), packet.payload, packet.seqnum);
} */

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  from_app_A++;
  if (seqnums_A - base_A >= MAX_BUFFER) {
    if(TRACE >1){
    printf("----A: New message arrives, send window is not full, send new messge to layer3!\n");
    exit(1);
    }
  } else {
    data_packet_A = create_pkt(seqnums_A, -1000, message.data);
    /* printf("at A, created packet with sequence number %d\n", seqnums_A); */
    queue[seqnums_A++] = data_packet_A;
    if (seqnums_A - base_A <= WINDOW_SIZE) {
        starttimer(seqnums_A-1, timeout_value);
        /* log_packet('A', data_packet_A); */
        /* printf("checksum A is %d\n", data_packet_A.checksum); */

        if (TRACE > 0)
        {
          printf("Sending packet %d to layer 3\n", data_packet_A.seqnum);
        }
            
        tolayer3(0, data_packet_A);
        from_transport_A++;
    }
    else {
      if (TRACE > 0)
      printf("----A: New message arrives, send window is full\n");
    }
    
  }
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  int checksum = packet.checksum;
  if (checksum == get_checksum(packet.seqnum, packet.acknum, packet.payload)) {
    if (packet.acknum != -1000)
    {
        printf("----A: uncorrupted ACK %d is received\n",packet.acknum);
        if (queue[packet.acknum].acknum != -1) {
          stoptimer(packet.acknum);
          queue[packet.acknum].acknum = -1;
          if (base_A == packet.acknum) {
            int packet_to_sent = base_A + WINDOW_SIZE;
            base_A++;
            while (queue[base_A].acknum == -1 && base_A < seqnums_A) {
              base_A++;
            }
            /* printf("base_A moved to %d\n", base_A); */
            while (packet_to_sent < seqnums_A && packet_to_sent < base_A + WINDOW_SIZE) {
              starttimer(packet_to_sent, timeout_value);
              /* log_packet('A', queue[packet_to_sent]); */
              tolayer3(0, queue[packet_to_sent++]);
            }
          }
        }
        
    } else {
        if (TRACE > 0)
        printf ("----A: duplicate ACK received, do nothing!\n");
    } 
  } else {
    if (TRACE > 0)
      printf ("----A: corrupted ACK is received, do nothing!\n");
  }
}

/* called when A's timer goes off */
void A_timerinterrupt(void)
{
  int packetType=0;
  if (TRACE > 0)
    printf("----A: time out,resend packets!\n");
  starttimer(packetType, timeout_value);
  /*log_packet('A', queue[packetType]);*/
  tolayer3(0, queue[packetType]);
  from_transport_A++;
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
  seqnums_A = 0;
  base_A = 0;
  timeout_value = 20.0;
}

/* called from layer 5, passed the data to be sent to other side */
void B_output(struct msg message)
{
}
/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
  int checksum;
  int i;
  struct pkt ack_packet;
  to_transport_B++;
  checksum = packet.checksum;
  if (checksum == get_checksum(packet.seqnum, packet.acknum, packet.payload)) 
  {
    if (packet.acknum != -1000)
    {
        /* printf("at B, something wrong acknum should be -1000 since unidirectional\n"); */
    } 
    else 
    {  
        if (TRACE > 0)
          printf("----B: packet %d is correctly received, send ACK!\n",packet.seqnum);
        if (packet.seqnum == last_received_B + 1) 
        {
          /* printf("last received B is %d\n", last_received_B);
            printf("at B, packet with message %.*s, seq number %d sent to layer 5 i.e. application layer\n", (int)sizeof(packet.payload), packet.payload,packet.seqnum); */
            to_app_B++;
            tolayer5(1, packet.payload);
            i=packet.seqnum%WINDOW_SIZE;
            receiver_buffer[i] = packet;
            receiver_buffer[i].acknum =  -1;
            i++;
            last_received_B++;
            i=i%WINDOW_SIZE;
            while (receiver_buffer[i].acknum !=  -1) {
              tolayer5(1, receiver_buffer[i].payload);
              /* printf("at B, packet with message %.*s and sequence number %d sent to layer 5 i.e. application layer\n", (int)sizeof(receiver_buffer[i].payload), receiver_buffer[i].payload, receiver_buffer[i].seqnum); */

              to_app_B++;

              receiver_buffer[i].acknum = -1;
              i++;
              i=i%WINDOW_SIZE;       
              last_received_B++;
            }          
        } 
        else 
        {
            if (TRACE > 0) 
              printf("----B: packet corrupted or not expected sequence number, resend ACK!\n");
            if (packet.seqnum > last_received_B + WINDOW_SIZE) {
              /* printf("at B, received packet number outside buffer window so ignoring packet %d.\n", packet.seqnum); */
              return;
            }
            else if (packet.seqnum > last_received_B){
              /* printf("at B, buffering the packet %d in window\n", packet.seqnum); */
              receiver_buffer[packet.seqnum%WINDOW_SIZE] = packet;
            }
        }
        ack_packet = create_pkt(-1000, packet.seqnum, NULL);
        /* printf("ack sent from B with acknum %d\n", ack_packet.acknum); */
        tolayer3(1, ack_packet);
    }
  }
  else {
    printf("corrupt packet detected at B\n");
  }
}

/* called when B's timer goes off */
void B_timerinterrupt()
{
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
  int i;
  last_received_B = -1;
  for (i=0; i < WINDOW_SIZE; i++) {
    receiver_buffer[i].acknum = -1;
  }
}