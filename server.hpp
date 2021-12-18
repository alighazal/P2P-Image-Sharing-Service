
#ifndef _SERVER_
#define _SERVER_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <string>
#include <iostream>
#include <mutex>  
#include <thread>
#include <vector>
#include <cstdlib>

#include <functional>


#include "json.hpp"
#include "requests_types.hpp"

using namespace std;

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 1024 * 100
#define MAX_KEYVAL 20



std::string lowerletters = "abcdefghijklmnopqrstuvwxyz";
std::string upperletters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

class Server{
  private:
    int server_socket, n, rc;
    socklen_t client_len;
    struct sockaddr_in client_address, server_address;
    std::mutex mtx;
    function< void(nlohmann::json) > func;


    void thread_function(int i){
      std::unique_lock<std::mutex> lck (mtx);
      //std::cout << "thread ("+ std::to_string(i) +") function\n";
      lck.unlock();

      char buffer_[MAX_MSG];
      memset(buffer_, 0x0, MAX_MSG);
      
      struct sockaddr_in client_address_ = GetRequest(buffer_);

      lck.lock();
      printf("thread %d - from %s:%u : %s \n",
            i, inet_ntoa(client_address_.sin_addr),
            ntohs(client_address_.sin_port), buffer_);
      lck.unlock();
      
      if(strcmp(buffer_, "q") == 0){
        exit(1);
      }

      nlohmann::json request = nlohmann::json::parse( buffer_ );
      
      this->func( request );
      
      SendReply(buffer_, client_address_);
    }

  public:

    Server( function< void(nlohmann::json) > func ): func(func) {}
    Server(){
      this->func = []( nlohmann::json t  ){
          cout << t << endl;
      };
    } 

    struct sockaddr_in GetRequest(char* buffer_){
      /* receive message */
      struct sockaddr_in client_address_;
      client_len = sizeof(client_address_);

      //int n;
      n = recvfrom(server_socket, buffer_, MAX_MSG, 0,
                  (struct sockaddr *)&client_address_, &client_len);
      
      return client_address_;
    }

    void SendReply(char* buffer_, struct sockaddr_in client_address_){
      //int rc;
      rc = sendto(server_socket, buffer_ , strlen(buffer_) + 1, 0,
                (struct sockaddr *)&client_address_,
                sizeof(client_address_));
    }

    void encrypt(char* buffer_){
      // Encrypting the message
      int key = (rand() % MAX_KEYVAL) +1;
      for(int i=0; i<strlen(buffer_); i++){
        if(isupper(buffer_[i])){
          int index = upperletters.find(buffer_[i]);
          buffer_[i] = upperletters[(index+key)%26];
        } else if (islower(buffer_[i])) {
          int index = lowerletters.find(buffer_[i]);
          buffer_[i] = lowerletters[(index+key)%26];
        }
      }
      char out_string[MAX_MSG+5];
      memset(out_string, 0x0, MAX_MSG+5);
      strcpy(out_string, (std::to_string(key)).c_str());
      strcat(out_string, "_");
      strcat(out_string, buffer_);
      strcpy(buffer_, out_string);
    }

    void connect(){
      /* socket creation */
      server_socket = socket(AF_INET, SOCK_DGRAM, 0);

      if (server_socket < 0)
      {
        printf("cannot open socket \n");
        exit(1);
      }

      /* bind local server port */
      server_address.sin_family = AF_INET;
      server_address.sin_addr.s_addr = htonl(INADDR_ANY);
      server_address.sin_port = htons(LOCAL_SERVER_PORT);

      if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        printf(" error with binding ");
    }

    void listen(){

      while (1)
      {
        std::cout << "execusion" << std::endl;
        std::vector<std::thread> threads_pool; 

        for ( int i = 0; i < 50; i++ )
          threads_pool.push_back( std::thread( &Server::thread_function, this, i ) );

        for (auto &t: threads_pool)
          t.join();
      } /* end of server infinite loop */

    }
};


#endif