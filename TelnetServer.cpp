#include "TelnetServer.h"

TelnetServer::TelnetServer(int port) : port(port) {
    sock = socket(PF_INET, SOCK_STREAM, 0); // creating IPv4 TCP socket
    if (sock < 0) {
        syserr("Error creating IPv4 TCP socket");
    }

    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
    server_address.sin_port = htons(port);
    client_address_len = sizeof(client_address);

    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        syserr("Error binding the socket to a concrete address");
    }
    if (listen(sock, QUEUE_LENGTH) < 0) {
        syserr("Error switching to listening");
    }
}

void TelnetServer::serve_clients() {
    while (true) {
        wait_for_next_client();
        set_telnet_options();
        clear_client_view();
        send_message_to_client(menu->get_string_representation() + message_after_menu);
        int len = 0;
        while ((len = read(msg_sock, buffer, sizeof(buffer))) >= 0) {
            std::string message(buffer, buffer + len);
            Action action = convert_client_message_to_action(message);
            if (action != OTHER) {
                menu->change_state(action);
                clear_client_view();
                if (!end_of_connection) {
                    send_message_to_client(menu->get_string_representation() + message_after_menu);
                } else {
                    break;
                }
            }
        }
        close_connection();
    }
}

void TelnetServer::close_connection() {
    std::cout << "Klient sie odlaczyl" << std::endl;
    if (close(msg_sock) < 0) {
        syserr("Error closing connection");
    }
}

void TelnetServer::wait_for_next_client() {
    msg_sock = accept(sock, (struct sockaddr *) &client_address, &client_address_len);
    if (msg_sock < 0) {
        syserr("Error accepting connection");
    }
    std::cout << "Klient sie podlaczyl" << std::endl;
    end_of_connection = false;
}

Action TelnetServer::convert_client_message_to_action(std::string message) {
    if (message == "\33\133\101") {
        return ARROW_UP;
    } else if (message == "\33\133\102") {
        return ARROW_DOWN;
    } else if (message.length() == 2 && message[0] == 13 && message[1] == 0) {
        return ENTER;
    } else {
        return OTHER;
    }
}

void TelnetServer::set_telnet_options() {
    //https://stackoverflow.com/questions/273261/force-telnet-client-into-character-mode
    send_message_to_client("\377\373\1\377\373\3\377\374\42");
}

void TelnetServer::send_message_to_client(std::string message) {
    if (write(msg_sock, message.c_str(), message.length()) < 0) {
        syserr("error writing");
    }
}

void TelnetServer::clear_client_view() {
    //Cursor movement: http://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
    send_message_to_client("\33[2J\33[0;0H");
}

void TelnetServer::set_menu(std::shared_ptr<Menu> menu) {
    this->menu = menu;
}

void TelnetServer::end_connection() {
    end_of_connection = true;
}

void TelnetServer::set_message_after_menu(std::string message) {
    message_after_menu = message;
}
