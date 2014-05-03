/******************************************************************************
 * tcp_client.c
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h> // some can be standard, others are ethernet based
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <SDL/SDL.h> // has a lot of joystick libraries
#include <time.h>
#include <unistd.h>

struct Motors {
   unsigned char motorA;
   unsigned char motorB;
   unsigned char motorC;
   unsigned char motorD;
   unsigned char motorServo;
   unsigned char motorClaw;
}  __attribute__((__packed__));

// for the server side
int tcp_recv_setup();
int tcp_listen(int server_socket, int back_log);

// for the client side
int tcp_send_setup(char *host_name, char *port);

void testButtons(SDL_Joystick *joy) {
   while(1)
   {
      SDL_JoystickUpdate();
         printf("Buttons: ");
      for(int i = 0; i < 20; i++) {
         printf("%d ", SDL_JoystickGetButton(joy, i));
      }
      printf("\n");
      usleep(10000);
   }
}

int main(int argc, char * argv[]) {

   SDL_Joystick *joy;
   Sint16 x_move, y_move, x2_move, y2_move;

   // Initialize the joystick subsystem
   SDL_InitSubSystem(SDL_INIT_JOYSTICK);

   printf("Checking for joysticks...\n");
   // Check for joystick
   if (SDL_NumJoysticks() > 0) {
      // Open joystick
      joy = SDL_JoystickOpen(0);

      testButtons(joy);

      // Getting hung up after the printf's...
      if (joy) {
         printf("Opened Joystick 0\n");
         printf("Name: %s\n", SDL_JoystickName(0));
         printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
         printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
         printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));

         //SDL_JoystickEventState(SDL_ENABLE);
         int socket_num;         //socket descriptor
         char *send_buf;         //data buffer
         int bufsize= 0;         //data buffer size
         int send_len= 0;        //amount of data to send
         int sent= 0;            //actual amount of data sent

         /* check command line arguments  */
         if (argc!= 3) {
            printf("usage: %s host-name port-number \n", argv[0]);
            exit(1);
         }
         /* set up the socket for TCP transmission  */
         socket_num= tcp_send_setup(argv[1], argv[2]);

         /* initialize data buffer for the packet */
         bufsize= 1024;
         send_buf= (char *) malloc(bufsize);

         /* get the data and send it   */
         printf("Enter the data to send: ");

         //send_len = 0;
         while ((send_buf[send_len] = getchar()) != '\n' && send_len < 80) {
            send_len++;
         }
   
         send_buf[send_len] = '\0';

         struct Motors motors;
         while(1) {
            usleep(1000);
            SDL_JoystickUpdate();

            if (SDL_JoystickGetButton(joy, 1)) {
               motors.motorServo = 127 + -128.0 * SDL_JoystickGetAxis(joy, 1) / 32768;
               motors.motorA = 127;
               motors.motorB = 127;
               motors.motorC = 127;
               motors.motorD = 127;
            }
            else if(SDL_JoystickGetButton(joy, 2)) {
               motors.motorClaw = 127 + -128.0 * SDL_JoystickGetAxis(joy, 1) / 32768;
               motors.motorA = 127;
               motors.motorB = 127;
               motors.motorC = 127;
               motors.motorD = 127;
            }
            else {
               x_move=SDL_JoystickGetAxis(joy, 0);
               y_move=SDL_JoystickGetAxis(joy, 1);

               motors.motorA =  127 + (-y_move + x_move) * 64.0 /32768;
               motors.motorB = 127 + (-y_move - x_move) * 64.0 /32768;
               //motors.motorA =  128 + (- 1 * x_move + - 1 * y_move) * 125.0 / 32768;

               x2_move=SDL_JoystickGetAxis(joy, 2);
               y2_move=SDL_JoystickGetAxis(joy, 3);

               motors.motorC =127 + (-y2_move + x2_move) * 64.0 /32768;
               motors.motorD = 127 + (-y2_move - x2_move) * 64.0 /32768;

               motors.motorClaw = 127;
               motors.motorServo = 127;
            }

            //printf("%d\n", y_move);
            printf("Motor A: %d, Motor B: %d, Motor C: %d, Motor D: %d, Servo: %d, Claw: %d\n",
               motors.motorA, motors.motorB, motors.motorC, motors.motorD, motors.motorServo, motors.motorClaw);

            /* now send the data */
            sent = send(socket_num, &motors, sizeof(Motors), 0);
            if (sent < 0) {
               perror("send call");
               exit(-1);
            }

            printf("String sent: %s \n", send_buf);
            printf("Amount of data sent is: %d\n", sent);

            usleep(100000);
         }
         
         close(socket_num);
      }
      else {
         printf("Couldn't open Joystick 0\n");
      }
      
      //Close if opened
      if(SDL_JoystickOpened(0)) {
         SDL_JoystickClose(joy);
      }
   }
   else {
      printf("No joysticks attached to the system.\n");
   }
}

int tcp_send_setup(char *host_name, char *port) {
   int socket_num;
   struct sockaddr_in remote;       // socket address for remote side
   struct hostent *hp;              // address of remote host

   // create the socket
   if ((socket_num = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket call");
      exit(-1);
   }

   // designate the addressing family
   remote.sin_family= AF_INET;

   // get the address of the remote host and store
   if ((hp = gethostbyname(host_name)) == NULL) {
      printf("Error getting hostname: %s\n", host_name);
      exit(-1);
   }
   
   memcpy((char*)&remote.sin_addr, (char*)hp->h_addr, hp->h_length);

   // get the port used on the remote side and store
   remote.sin_port= htons(atoi(port));

   if(connect(socket_num, (struct sockaddr*)&remote, sizeof(struct sockaddr_in)) < 0) {
      perror("connect call");
      exit(-1);
   }
   
   return socket_num;
}

