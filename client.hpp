
#ifndef _CLIENT_
#define _CLIENT_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>   /* memset() */
#include <sys/time.h> /* select() */
#include <string>
#include <iostream>
#include "json.hpp"




#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 250

// argv[0] --> ./client
// argv[1] --> server ip
// argv[2] --> message

class Client{
  private:
    std::string lowerletters = "abcdefghijklmnopqrstuvwxyz";
    std::string upperletters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int sd , rc, i;
    struct sockaddr_in cliAddr, remoteServAddr;
    struct hostent *h;
    char buffer[MAX_MSG];
    socklen_t client_len;
  public:
  
    Client(){};

    void connect(char* server_ip){
      struct hostent *h = gethostbyname(server_ip);
      if (h == NULL)
      {
        printf("unknown host '%s' \n", server_ip);
        exit(1);
      }

      printf("sending data to '%s' (IP : %s) \n", h->h_name,
            inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

      remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);
      remoteServAddr.sin_family = h->h_addrtype;
      memcpy((char *)&remoteServAddr.sin_addr.s_addr,
            h->h_addr_list[0], h->h_length);
      /* socket creation */
      sd = socket(AF_INET, SOCK_DGRAM, 0);
      
      if (sd < 0)
      {
        printf("cannot open socket \n");
        exit(1);
      }

      /* bind any port */
      cliAddr.sin_family = AF_INET;
      cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      cliAddr.sin_port = htons(1700);

      rc = bind(sd, (struct sockaddr *)&cliAddr, sizeof(cliAddr));
      if (rc < 0)
      {
        printf("cannot bind port\n");
        exit(1);
      }
    }

    void send(char* message){
      /* send data */
      rc = sendto(sd, message, strlen(message) + 1, 0,
                  (struct sockaddr *)&remoteServAddr,
                  sizeof(remoteServAddr));
    }

    std::string recieve(){
      //// reciving response
      memset(buffer, 0x0, MAX_MSG);

      /* receive message */
      client_len = sizeof(cliAddr);

      int n = recvfrom(sd, buffer, MAX_MSG, 0,
                    (struct sockaddr *)&remoteServAddr, &client_len);

      printf("From %s:%u : %s \n", inet_ntoa(remoteServAddr.sin_addr),
              ntohs(remoteServAddr.sin_port), buffer);
      //decrypt(buffer);
      //printf("Decrypted: %s \n", buffer);

      if (rc < 0)
      {
        printf("cannot send data %d \n", i - 1);
        close(sd);
        exit(1);
      }

      return std::string(buffer);

    }

    void decrypt(char* buffer_){
      bool key_found = false;
      int key;
      int start_index;
      for(int i=0; i<strlen(buffer_); i++){
        if(~key_found && (buffer_[i] == '_') ){
            char key_str [i];
            start_index = i+1;
            strncpy(key_str, buffer_, i);
            key = atoi(key_str);
            key_found = true;
        } else if(isupper(buffer_[i])){
          int index = upperletters.find(buffer_[i]);
          buffer_[i] = upperletters[(index-key+26)%26];
        } else if (islower(buffer_[i])) {
          int index = lowerletters.find(buffer_[i]);
          buffer_[i] = lowerletters[(index-key+26)%26];
        }
      }
      memmove(buffer_, buffer_+start_index, strlen(buffer_) - start_index + 1);
    }

};


#endif