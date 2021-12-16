#include "client.hpp"
#include "base64.h"

#include <fstream>

using namespace std;

#include "requests_types.hpp"

void get_list_of_images( Client client){
  nlohmann::json request_images;

  request_images["request_type"] = RequestType::GET_IMAGES_LIST;
  request_images["user_id"] = "2";

  client.send(/*message*/ (char *)request_images.dump().c_str());

  string out = client.recieve();

  cout << " ------> " << out << endl;

  nlohmann::json reply = nlohmann::json::parse(out);

  for (auto& image_id: reply["images_list"] ){
    // send request to get each of those images
    cout << " --> " <<  stoi(string(image_id)) << endl;
  }

  
}

int main(int argc, char *argv[])
{
  /* check command line args */
  if (argc < 2)
  {
    printf("usage : %s <server_ip> \n", argv[0]);
    exit(1);
  }

  char *server_ip = argv[1];
  Client client = Client();
  client.connect(server_ip);

  nlohmann::json user;

  user["request_type"] = RequestType::ADD_USER;
  user["first_name"] = "ali";
  user["last_name"] = "mohamed";
  user["email"] = "ali@emial.com";
  user["password"] = "123456";

  client.send(/*message*/ (char *)user.dump().c_str());

  user["request_type"] = RequestType::ADD_USER;
  user["first_name"] = "Laila";
  user["last_name"] = "Mohamed";
  user["email"] = "Laila@emial.com";
  user["password"] = "123456";

  client.send(/*message*/ (char *)user.dump().c_str());

  nlohmann::json image;

  std::ifstream ifs("./images/test_image.jpg");
  if(!ifs)
    throw invalid_argument(" can't load the image ... file path corrupted ");

  std::string image_bytes = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
  string imags_bytes_base64  =  base64_encode(image_bytes, false);


  image["request_type"] = RequestType::ADD_IMAGE;
  image["content"] =  imags_bytes_base64;
  image["user_id"] = "1";


  client.send(/*message*/ (char *)image.dump().c_str());



  nlohmann::json image_permission;

  image_permission["request_type"] = RequestType::GIVE_PERMISSION;
  image_permission["image_id"] = "1";
  image_permission["sent_from"] = "1";
  image_permission["sent_to"] = "2";
  image_permission["view_count"] = "5";

  client.send(/*message*/ (char *)image_permission.dump().c_str());

  image_permission["request_type"] = RequestType::GIVE_PERMISSION;
  image_permission["image_id"] = "2";
  image_permission["sent_from"] = "1";
  image_permission["sent_to"] = "2";
  image_permission["view_count"] = "5";

  client.send(/*message*/ (char *)image_permission.dump().c_str());


  // get list of images
  get_list_of_images( client );
  



  // j["request_type"] = RequestType::VERIFY;
  // client.send(/*message*/ (char *)j.dump().c_str());

  // j["request_type"] = RequestType::GET;
  // client.send(/*message*/ (char *)j.dump().c_str());

  // j["request_type"] = RequestType::GET_ALL;
  // client.send(/*message*/ (char *)j.dump().c_str());

  // j["request_type"] = RequestType::DELETE;
  // client.send(/*message*/ (char *)j.dump().c_str());
}
