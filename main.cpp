#include <iostream>
#include "Menu.h"
#include "TelnetServer.h"

int parse_int(const char *arg) {
    try {
        return std::stoi(arg);
    } catch(std::exception const & e) {
        std::cerr << "Argument should be integer\n";
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port\n";
        exit(1);
    }
    int port = parse_int(argv[1]);

    TelnetServer server(port);
    std::shared_ptr<Menu> root = std::make_shared<Menu>([](){});

    std::shared_ptr<Menu> a = std::make_shared<Menu>([&server](){
        server.set_message_after_menu("A");
    }, "Opcja A");
    root->add_submenu(a);

    std::shared_ptr<Menu> b = std::make_shared<Menu>([&b, &server](){
        server.set_message_after_menu("");
        b->get_parent()->set_position(0);
        server.set_menu(b);
    }, "Opcja B");
    root->add_submenu(b);

    std::shared_ptr<Menu> b1 = std::make_shared<Menu>([&server](){
        server.set_message_after_menu("B1");
    }, "Opcja B1");
    b->add_submenu(b1);

    std::shared_ptr<Menu> b2 = std::make_shared<Menu>([&server](){
        server.set_message_after_menu("B2");
    }, "Opcja B2");
    b->add_submenu(b2);

    std::shared_ptr<Menu> go_back_b = std::make_shared<Menu>([&go_back_b, &server](){
        server.set_menu(go_back_b->get_parent()->get_parent());
        go_back_b->get_parent()->set_position(0);
        server.set_message_after_menu("");
    }, "Powrot");

    b->add_submenu(go_back_b);

    std::shared_ptr<Menu> end = std::make_shared<Menu>([&end, &server](){
        end->get_parent()->set_position(0);
        server.set_message_after_menu("");
        server.end_connection();
    }, "Koniec");
    root->add_submenu(end);

    server.set_menu(root);
    server.serve_clients();
}