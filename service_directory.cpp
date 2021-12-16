#include "server.hpp"
#include "sql_wrapper.hpp"

void print ( string s){
  cout << s << endl;
}

int create_tables( string database_name, sqlite3 *DB ){

  string  sql_statment = "ss";

  int exit = 0;
  exit = sqlite3_open(database_name.c_str(), &DB);

  char *messaggeError;
  exit = sqlite3_exec(DB, sql_statment.c_str(), NULL, 0, &messaggeError);

  if (exit != SQLITE_OK)
  {
      std::cerr << "Error Create Table" << std::endl;
      sqlite3_free(messaggeError);
      return -1;
  }
  else
      std::cout << "Table created Successfully" << std::endl;

  sqlite3_close(DB);

  return 0;

}


void dispactcher( nlohmann::json request ){


      if (request["request_type"] == nullptr ){
          throw invalid_argument(" request type not specified ");
      }

      int request_type = request["request_type"];

      switch ( request_type )
      {
      case RequestType::ADD :
        cout << " --> Add request" <<   endl;
        print(" alii ");
        break;
      case RequestType::EDIT :
        cout << " --> EDIT request" <<   endl;
        break;
      case RequestType::GET :
        cout << " --> GET request" <<   endl;
        break;
      case RequestType::GET_ALL :
        cout << " --> GET_ALL request" <<   endl;
        break;
      default:
        cout << " --> undefined request" <<   endl;
        break;
      }
}

int main(int argc, char *argv[])
{

  sqlite3 *DB;
  SqlWrapper db(DB, "test.db");
  int rc = db.connect();

  
  Server server (dispactcher) ;
  server.connect();
  server.listen();
}