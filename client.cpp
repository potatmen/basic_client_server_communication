#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

void info(string address, int port, int send_status, int connect_status) { // printing data about connection status takes IP, PORT, send status and connect status
  cout << "IP Address: " << address << "   Port Number: " << port
       << endl; // server data and port number
  string con_stat = connect_status == -1 ? "ERROR" : "OK"; // connection status
  string se_stat = send_status == -1 ? "ERROR" : "OK";     // send status
  cout << "Connect status: " << con_stat << endl
       << "Send status: " << se_stat << endl;
}

int main(int argc, char **argv) {
  const char *address = argv[1]; // IP address of server from input
  int PORT = stoi(argv[2]);      // PORT of server from input
  int sock, client_fd;
  struct sockaddr_in serv_addr; // stctuct storing information about server
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // checking whether we can create socket and init socket with TCP connection
    cout << "Socket creation error" << endl;
    return -1;
  }
  serv_addr.sin_family = AF_INET; // IPv4 addresses are used
  serv_addr.sin_port =
      htons(PORT); // translation of port number to hexadecimal number
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // getting the IP address of server
  if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) { // checking whether IP format is correct
    cout << "Invalid address/ Address not supported" << endl;
    return -1;
  }
  if ((client_fd =
           connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) { // checking whether connection is established
    cout << "Connection Failed" << endl;
    return -1;
  }
  int cnt = 1;  // counter of number of sent POST messages
  char *buffer; // storing the data sent from server
  while (true) {
    buffer = new char[4096]; // updating buffer with size of 4096 bytes
    cout << "client: ";
    string s;
    cin >> s; // storing the user's input
    int status = send(sock, &s, s.length(),
                      0); // sending message and getting the sending status
    if (s == "POST") {
      info(address, PORT, client_fd, status); // printing info about sent message
      while (s != "#") { // getting messages from user and sending them one by one
        cnt++;
        cout << "client: ";
        cin >> s;
        send(sock, &s, s.length(), 0); // sending message
      }
      read(sock, buffer, 4096); // reading the data sent from server
      cout << buffer << endl;
      cnt--; // subtracting # symbol from number of sent messages
    } else if (s == "READ") {
      info(address, PORT, client_fd, status); // printing info about sent message
      for (int i = 0; i < cnt; ++i) { // reading data from server for READ function
        buffer = new char[4096];
        read(sock, buffer, 4096);
        cout << buffer << endl;
      }
    } else if (s == "QUIT") {   // handling QUIT option
      read(sock, buffer, 4096); // reading data from server
      cout << buffer << endl;
      break; // finishing the while function to close the socket
    } else {
      read(sock, buffer, 4096); // sending data to server if current message is not one of the above
      cout << buffer << endl;
    }
  }
  close(client_fd); // closing connection
  return 0;
}
