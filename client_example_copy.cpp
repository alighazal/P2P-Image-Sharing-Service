#include "client.hpp"
#include "base64.h"
#include <fstream>

using namespace std;

#include "requests_types.hpp"

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

  user["request_type"] = RequestType::ADD;
  user["first_name"] = "ali";
  user["last_name"] = "mohamed";
  user["email"] = "ali@emial.com";
  user["password"] = "123456";

  client.send(/*message*/ (char *)user.dump().c_str());

  nlohmann::json image;

  std::ifstream ifs("./images/test_image.jpg");
  if(!ifs)
    throw invalid_argument(" can't load the image ... file path corrupted ");

  std::string image_bytes = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
  string imags_bytes_base64  =  base64_encode(image_bytes, false);

  //cout << imags_bytes_base64 << endl;

  image["request_type"] = RequestType::ADD_IMAGE;
  //image["content"] = "/9j/4AAQSkZJRgABAQEAYABgAAD/4QBoRXhpZgAATU0AKgAAAAgABAEaAAUAAAABAAAAPgEbAAUAAAABAAAARgEoAAMAAAABAAIAAAExAAIAAAARAAAATgAAAAAAAXcLAAAD6AABdwsAAAPoUGFpbnQuTkVUIHYzLjUuNwAA/9sAQwAMCQkLCQgMCwoLDg0MDxMfFBMRERMmGx0XHy0oMC8sKCwrMjhIPTI1RDYrLD5VP0RKTFBRUDA8WF5XTl5IT1BN/9sAQwENDg4TEBMlFBQlTTMsM01NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1NTU1N/8AAEQgBLAEsAwEiAAIRAQMRAf/EAB8AAAEFAQEBAQEBAAAAAAAAAAABAgMEBQYHCAkKC//EALUQAAIBAwMCBAMFBQQEAAABfQECAwAEEQUSITFBBhNRYQcicRQygZGhCCNCscEVUtHwJDNicoIJChYXGBkaJSYnKCkqNDU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6g4SFhoeIiYqSk5SVlpeYmZqio6Slpqeoqaqys7S1tre4ubrCw8TFxsfIycrS09TV1tfY2drh4uPk5ebn6Onq8fLz9PX29/j5+v/EAB8BAAMBAQEBAQEBAQEAAAAAAAABAgMEBQYHCAkKC//EALURAAIBAgQEAwQHBQQEAAECdwABAgMRBAUhMQYSQVEHYXETIjKBCBRCkaGxwQkjM1LwFWJy0QoWJDThJfEXGBkaJicoKSo1Njc4OTpDREVGR0hJSlNUVVZXWFlaY2RlZmdoaWpzdHV2d3h5eoKDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uLj5OXm5+jp6vLz9PX29/j5+v/aAAwDAQACEQMRAD8A9VooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAoqFbqFvMIkXbGcM2eAfrVCfXraNtsQeZv9kYFXGnOTskZTr04K8pGrRUay4iRpcRswyVJ6H0p4YMMqQQe4qbWNE0xaKKKQwooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooqhc6tDaXXkTKwGAdw5FVGLk7RInUjTV5OxfoqKG5huF3QyK49j0qWk01oyk01dBRRRSGFFFFABRRRQAUUUUAFc7q2sFy1vathBwzjv7D2qxrmo+Un2WFvnYfOR2HpXOV6GGofbkeNj8Y0/ZQfr/kPMrmMR7jsBzt7Z9aI5HibdGdrdiOo+lMqxBezW3+pKqfXYpP54rua00R5MXr7zJIrC9vH3CNzn+N+B+ZrpdNszY2oiZ97E7iR0H0rFi8QXSH96qSD6YNatprNtdEKSYpD/C/f6GuDEe1as1p5Hr4J4aMrqXveZo0UUVwHsBRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUAFFFFABXO+I4CJYpwOCNh+o/z+ldFVe8tUvLZ4X4z0PofWtaNT2c1I58VR9tScVucYrsjBkYqw6EHBrTtteuYcCXEy+/B/Os6eCS2maKVcMpqOvXlCFRaq583CrUov3XZnXWur2l1gb/Lc/wvx+tX64Krlrqd1aYEcm5P7jciuOpg+sGelRzPpVXzR2NFZVprtvPhZv3L+5+U/jWoCCAQcg9xXFOEoO0ketTqwqq8HcWiiioNAqveXS2dq8zc4HA9T2qxXM69d+dciBT8kXX3atqFP2k7HNiq/sabl16GVJI0sjSOcsxyTTaKK9k+XbvqwooooEFFFFAGtpusvbFYrgl4egPUr/8AWrpUdZEDoQysMgjvXCVr6LqRt5Rbyt+6c/KT/Cf8K4sRh01zx3PVwWNcWqdR6HTUUUV5p7oUUUUAFFFFABRRRQAUUUUAFFFFABRRRQAUUUUA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V2xIqL6KMVzTxkV8Kud1LLJvWbsc/aeH5HIa6fy1/urya3be1htE2QRhR3Pc/U1NRXFUrTqbs9WjhadH4Vr3CiiisjoCiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooA//2Q==";
  image["content"] = imags_bytes_base64;
  image["user_id"] = "1";

  cout << (char *)image.dump().c_str() << endl;


  client.send(/*message*/ (char *)image.dump().c_str());

  // j["request_type"] = RequestType::VERIFY;
  // client.send(/*message*/ (char *)j.dump().c_str());

  // j["request_type"] = RequestType::GET;
  // client.send(/*message*/ (char *)j.dump().c_str());

  // j["request_type"] = RequestType::GET_ALL;
  // client.send(/*message*/ (char *)j.dump().c_str());

  // j["request_type"] = RequestType::DELETE;
  // client.send(/*message*/ (char *)j.dump().c_str());
}
