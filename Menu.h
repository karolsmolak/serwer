#ifndef SERWER_MENU_H
#define SERWER_MENU_H

#include <functional>
#include "TelnetServer.h"
#include "Action.h"

class Menu {
    std::shared_ptr<Menu> parent;
    std::vector<std::shared_ptr<Menu>> submenus;
    int current_position;
    std::string title;
    std::function<void()> on_select;
public:
    Menu(std::function<void()> on_select, std::string title = "root") : current_position(0), title(title), on_select(on_select) {}

    void change_state(Action action);

    std::string get_string_representation();

    void add_submenu(std::shared_ptr<Menu> menu);

    std::shared_ptr<Menu> get_parent();

    void set_position(int position);
};


#endif //SERWER_MENU_H
