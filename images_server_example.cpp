#include <iostream>
#include <fstream>
#include <sqlite3.h>

#include "images_server.hpp"

using namespace std;

int main()
{
    images_server is;

    is.connect();
    is.listern_and_server();

    return 0;
}