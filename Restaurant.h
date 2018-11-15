//
// Created by Amit Levizky on 11/3/18.
//

#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    Restaurant(const Restaurant &other);
    virtual ~Restaurant();
    Restaurant(Restaurant &&other);
    Restaurant& operator=(const Restaurant &other);
    Restaurant& operator=(Restaurant &&other);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    DishType findMyType(std::string strType);
};
#endif //RESTAURANT_H_
