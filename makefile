server: 
	g++ images_server_example.cpp base64.cpp -o images_server_example -l pthread -l sqlite3; ./images_server_example

client: 
	g++ client_example.cpp base64.cpp -o client; ./client localhost

clean: 
	rm ./images_server_example ./client