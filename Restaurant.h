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
    int nextCustomerId;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    DishType findMyType(std::string strType);
    void goOpen(std::string details);
    void goOrder(std::string details);
    void goMove(std::string details);
    void goClose(std::string details);
    void goCloseAll();
    void goMenu();
    void goStatus(std::string details);
    void goLog();
    void goBackup();
    void goRestor();
    Customer* createCustomerByType(std::string name, std::string strategy);



};
#endif //RESTAURANT_H_
