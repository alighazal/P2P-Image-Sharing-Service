#include "server.hpp"
#include <fstream>


int main(int argc, char *argv[])
{

  //loading database



  Server server;
  server.connect();
  server.listen();
}