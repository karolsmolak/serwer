#ifndef SERWER_TELNETSERVER_H
#define SERWER_TELNETSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <memory>
#include <iostream>
#include "Menu.h"
#include "Action.h"
#include "err.h"

#define BUFFER_SIZE   10
#define QUEUE_LENGTH     5

class Menu;

class TelnetServer {
    int sock, msg_sock;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len;
    int port;
    std::shared_ptr<Menu> menu;
    unsigned char buffer[BUFFER_SIZE];
    bool end_of_connection;
    std::string message_after_menu;

    void set_telnet_options();
    Action convert_client_message_to_action(std::string message);
    void wait_for_next_client();
    void close_connection();
    void clear_client_view();

public:
    TelnetServer(int port);

    void send_message_to_client(std::string message);

    void serve_clients();

    void set_menu(std::shared_ptr<Menu> menu);

    void end_connection();

    void set_message_after_menu(std::string message);
};


#endif //SERWER_TELNETSERVER_H
