#include "Menu.h"

void Menu::change_state(Action action) {
    if (action == ARROW_DOWN) {
        current_position = (current_position + 1 + submenus.size()) % submenus.size();
    }
    if (action == ARROW_UP) {
        current_position = (current_position - 1 + submenus.size()) % submenus.size();
    }
    if (action == ENTER) {
        submenus[current_position]->on_select();
    }
}

std::string Menu::get_string_representation() {
    std::string result;
    for (int i = 0 ; i < submenus.size() ; i++) {
        if (current_position == i) {
            result += "- ";
        }
        result += submenus[i]->title + "\n\r";
    }
    return result;
}

void Menu::add_submenu(std::shared_ptr<Menu> menu) {
    submenus.push_back(menu);
    menu->parent = std::shared_ptr<Menu>(this);
}

std::shared_ptr<Menu> Menu::get_parent() {
    return parent;
}

void Menu::set_position(int position) {
    current_position = position;
}
