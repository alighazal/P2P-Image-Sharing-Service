
#include "server.hpp"
#include "base64.h"

#include <fstream>

class images_server : public Server
{

    // TODO HANDLE RESPONSES

private:
    std::mutex mtx;
    sqlite3 *DB;
    string database_name;

    void add_new_user_to_db(nlohmann::json request)
    {

        char sql_statment[MAX_MSG];

        char *messaggeError;
        int exit = sqlite3_open("images_server.db", &this->DB);

        sprintf(sql_statment, "insert into users ( first_name, last_name, email, password )"
                              "values ( \'%s\', \'%s\', \'%s\', \'%s\' ) ",
                string(request["first_name"]).c_str(),
                string(request["last_name"]).c_str(),
                string(request["email"]).c_str(),
                string(request["password"]).c_str());

        exit = sqlite3_exec(DB, sql_statment, NULL, 0, &messaggeError);
        if (exit != SQLITE_OK)
        {
            std::cerr << "Error Insert" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "Records created Successfully!" << std::endl;

        cout << sql_statment << endl;
    }

    void verify_user(nlohmann::json request)
    {
        char sql_statment[MAX_MSG];

        char *messaggeError;
        int exit = sqlite3_open("images_server.db", &this->DB);

        sprintf(sql_statment, "select * from users where email= '%s' and password = '%s' ",
                string(request["email"]).c_str(),
                string(request["password"]).c_str());

        auto callback = [](void *data, int argc, char **argv, char **azColName){
            *(bool *)data = (bool)true;
            return 0;
        };


        bool exists =  false;
        cout << "before: " << exists << endl;  
        
        exit = sqlite3_exec(DB, sql_statment, callback, &exists , NULL);
        
        cout << "after: " << exists << endl; 

        if (! exists )
            throw invalid_argument("user does not exist");
        
    }

    void add_image_to_db( nlohmann::json request ){

        char sql_statment[MAX_MSG];

        char *messaggeError;
        int exit = sqlite3_open("images_server.db", &this->DB);

        sprintf(sql_statment, "insert into images ( content, user_id )"
                              "values ( \'%s\', \'%s\' ) ",
                string(request["content"]).c_str(),
                string(request["user_id"]).c_str());

        exit = sqlite3_exec(DB, sql_statment, NULL, 0, &messaggeError);
        if (exit != SQLITE_OK)
        {
            std::cerr << "Error Inserting the image" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "image added created Successfully!" << std::endl;

        cout << sql_statment << endl;
    
        string out_image = base64_decode(string(request["content"]));

        cout << out_image << endl;

        ofstream wf("image.jpg", ios::out | ios::binary);

        wf << out_image; 

        cout << "done writing to a file " << endl;

    }


    void give_permission_to_user( nlohmann::json request ){

        char sql_statment[MAX_MSG];

        char *messaggeError;
        int exit = sqlite3_open("images_server.db", &this->DB);

        sprintf(sql_statment, "insert into images_permission ( image_id, send_from, sent_to, view_count )"
                              "values ( \'%s\', \'%s\', \'%s\', \'%s\' ) ",
                string(request["image_id"]).c_str(),
                string(request["sent_from"]).c_str(),
                string(request["sent_to"]).c_str(),
                string(request["view_count"]).c_str());

        exit = sqlite3_exec(DB, sql_statment, NULL, 0, &messaggeError);
        if (exit != SQLITE_OK)
        {
            std::cerr << "Error giving the permission" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "permissions was created Successfully!" << std::endl;

        cout << sql_statment << endl;
    }

    void get_users_viewable_images( nlohmann::json request, struct sockaddr_in client_address_ ){

        char sql_statment[MAX_MSG];

        char *messaggeError;
        int exit = sqlite3_open("images_server.db", &this->DB);

        sprintf(sql_statment, "select  images_permission.image_id from images_permission where images_permission.sent_to = %s ; ",
                string(request["user_id"]).c_str());

        auto callback = [](void *data, int argc, char **argv, char **azColName){

            unordered_map<string, string> output;

            for (int i = 0; i < argc; i++)
            {
                output[ azColName[i]  ] = argv[i] ? argv[i] : "NULL";
            }

            vector<unordered_map<string, string>>& data_vector = *reinterpret_cast<vector<unordered_map<string, string>>*>(data);

            data_vector.push_back( output );

            return 0;
        };


        vector<unordered_map<string, string>> data_vector;

        exit = sqlite3_exec(DB, sql_statment, callback, &data_vector , NULL);

        vector<string> images_id; 
        for ( auto& record: data_vector ){
            images_id.push_back( record["image_id"]);
            for ( auto& field: record  ){
                cout << field.first << " -- " << field.second << endl;
            }
            cout << endl;
        }

        nlohmann::json reply;

        reply["images_list"] = images_id;

        this->SendReply( (char *)reply.dump().c_str(), client_address_ );

    }

    void get_image ( nlohmann::json request ){



    }


    void handleRequests(char *buffer, struct sockaddr_in client_address_)
    {

        nlohmann::json request = nlohmann::json::parse(buffer);

        cout << request << endl;

        if (request["request_type"] == nullptr)
        {
            throw invalid_argument("request types was not included in the request");
        }

        int request_type = request["request_type"];

        switch (request_type)
        {
        case RequestType::ADD_USER:
            cout << "add" << endl;
            this->add_new_user_to_db(request);
            break;
        case RequestType::VERIFY:
            cout << "VERIFY" << endl;
            this->verify_user(request);
            //cout << "yess ???" << endl; 
            break;

        case RequestType::ADD_IMAGE:
            cout << "adding image to db" << endl;
            this->add_image_to_db( request  );
            break;

        case RequestType::GIVE_PERMISSION:
            cout << "giving permission" << endl;
            this->give_permission_to_user( request  );
            break;

        case RequestType::GET_IMAGES_LIST:
            cout << "getting images list" << endl;
            this->get_users_viewable_images( request , client_address_);
            break;

        case RequestType::GET_IMAGE:
            cout << "get" << endl;
            this->get_image( request );
            break;
        case RequestType::GET_ALL:
            cout << "get_all" << endl;

            break;
        case RequestType::EDIT:
            cout << "edit" << endl;

            break;
        case RequestType::DELETE:
            cout << "delete" << endl;

            break;

            

        default:
            break;
        }
    }

public:
    images_server()
    {
        std::cout << "i am an image server" << endl;

        // loading sql schema
        string sql_create_statments = "";
        std::ifstream file("schema.sql");

        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                sql_create_statments += line + '\n';
            }
            file.close();
        }

        // creating that database
        int exit = 0;
        exit = sqlite3_open("images_server.db", &DB);

        char *messaggeError;
        exit = sqlite3_exec(DB, sql_create_statments.c_str(), NULL, 0, &messaggeError);

        if (exit != SQLITE_OK)
        {
            std::cerr << "Error Create Table" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "Table created Successfully" << std::endl;

        sqlite3_close(DB);
    }

    void listern_and_server()
    {
        while (1)
        {
            char buffer_[MAX_MSG];
            memset(buffer_, 0x0, MAX_MSG);

            struct sockaddr_in client_address_ = GetRequest(buffer_);

            printf("from %s:%u : %s \n",
                   inet_ntoa(client_address_.sin_addr),
                   ntohs(client_address_.sin_port), buffer_);

            if (strcmp(buffer_, "q") == 0)
            {
                exit(1);
            }

            this->handleRequests(buffer_, client_address_);

            //SendReply(buffer_, client_address_);

        } /* end of server infinite loop */
    }
};