//
// Created by Amit Levizky on 11/3/18.
//
#include "Restaurant.h"

Restaurant::Restaurant() : open(true) {}

Restaurant::Restaurant(const std::string &configFilePath) : open(true){}

void Restaurant::start(){}

int Restaurant::getNumOfTables() const {
    return tables.size();
}

Table* Restaurant::getTable(int ind) {
    if (!(ind > tables.size() || ind < 0))
        return tables[ind];
    return nullptr;
}

std::vector<Dish>& Restaurant::getMenu() {return menu; }

const std::vector<BaseAction*>& Restaurant::getActionsLog() const { return actionsLog;}