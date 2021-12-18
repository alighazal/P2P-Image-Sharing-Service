#include "client.hpp"
#include "base64.h"

#include "reply_types.hpp"

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


bool verify_user( Client client,  nlohmann::json verify ){

  client.send(/*message*/ (char *)verify.dump().c_str());

  string reply_body = client.recieve();

  nlohmann::json reply = nlohmann::json::parse(reply_body);

  if ( reply["status"] == ReplyType::SUCCESS ){
      cout << " !! verified !! " << endl;
      return true;
  } else{
      cout << " !! not verified !! " << endl;
      return false;
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

  //client.send(/*message*/ (char *)image_permission.dump().c_str());

  image_permission["request_type"] = RequestType::GIVE_PERMISSION;
  image_permission["image_id"] = "2";
  image_permission["sent_from"] = "1";
  image_permission["sent_to"] = "2";
  image_permission["view_count"] = "5";

  //client.send(/*message*/ (char *)image_permission.dump().c_str());


  // get list of images
  get_list_of_images( client );


  nlohmann::json verify;

  verify["request_type"] = RequestType::VERIFY;
  verify["email"] = "Laila@emial.com";
  verify["password"] = "123456";

  verify_user( client,  verify );

  verify["request_type"] = RequestType::VERIFY;
  verify["email"] = "Laila@emial.com";
  verify["password"] = "123456xx"; // wrong password

  verify_user( client,  verify );



  // get one image

  nlohmann::json get_image;

  get_image["request_type"] = RequestType::GET_IMAGE;
  get_image["image_id"] = "1";
  get_image["email"] = "Laila@emial.com";


  client.send(/*message*/ (char *)get_image.dump().c_str());

  string out = client.recieve();
  
  cout << " ------> " << out << endl;

  nlohmann::json reply = nlohmann::json::parse( out );
  
  if ( reply["status"] != ReplyType::FAILURE ){
    string out_image = base64_decode(string(reply["images_content"]));
    ofstream wf("image_test_test_test.jpg", ios::out | ios::binary);
    wf << out_image; 
    cout << "done writing to a file " << endl;
  }else{
    cout << "sorry, image does not exit " << endl;

  }



}
