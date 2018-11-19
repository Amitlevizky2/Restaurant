//
// Created by Amit Levizky on 11/3/18.
//
#include "Restaurant.h"
#include <vector>
#include <iostream>
#include <fstream>

Restaurant::Restaurant() : open(true), nextCustomerId(0), tables(), menu(), actionsLog(), newMsg(""){}

Restaurant::Restaurant(const std::string &configFilePath) : open(true), nextCustomerId(0), tables(), menu(), actionsLog(), newMsg("")
{
    std::ifstream myfile(configFilePath);
    std::string line;
    std::string delimeter = ",";
    int numOfTables = INT8_MAX;

    if(myfile.is_open())
    {
        while (getline(myfile, line))
        {
            line  += ",";
            size_t pos = 0;
            std::string token;
            std::vector<std::string> newDish;
            while ((pos = line.find(delimeter)) != std::string::npos && line[0] != '#')
            {
                int tablesSize = tables.size();

                token = line.substr(0, pos);
                if(numOfTables == INT8_MAX)
                {
                    numOfTables = std::stoi(token);
                    line.erase(0, pos + delimeter.length());
                }
                else if(tablesSize < numOfTables)
                {
                    auto *t = new Table(std::stoi(token));
                    tables.push_back(t);
                    line.erase(0, pos + delimeter.length());
                }
                else
                {

                    newDish.push_back(token);
                    line.erase(0, pos + delimeter.length());
                }
                if(newDish.size() == 3)
                {
                    DishType menuDish = findMyType(newDish[1]);
                    Dish toBeAdd(static_cast<int>(menu.size()), newDish[0], std::stoi(newDish[2]), menuDish);
                    menu.push_back(toBeAdd);
                    newDish.clear();
                }
            }
        }
    }
}



Restaurant::Restaurant(const Restaurant &other) :open(other.open), nextCustomerId(other.nextCustomerId),  tables(), menu(other.menu), actionsLog(), newMsg("")
{
    for (auto table : other.tables) {
        auto * tempTable = new Table(*table);
        tables.push_back(tempTable);
    }
    for (auto j : other.actionsLog) {
        actionsLog.push_back(j->clone());
    }
}

Restaurant::~Restaurant()
{
    for (auto &table : tables) {
        delete table;
    }
    for (auto &j : actionsLog) {
        delete j;
    }
    tables.clear();
    actionsLog.clear();
    menu.clear();
}

Restaurant::Restaurant(Restaurant &&other) : open(other.open), nextCustomerId(other.nextCustomerId), tables(std::move(other.tables)), menu(std::move(other.menu)), actionsLog(std::move(other.actionsLog)), newMsg("")
{
    other.tables.clear();
    other.actionsLog.clear();
    other.menu.clear();
}

Restaurant &Restaurant::operator=(const Restaurant &other) {
    if (this == &other)
        return *this;
    open = other.open;
    nextCustomerId=other.nextCustomerId;
    for (auto &table : tables) {
        delete table;
    }
    tables.clear();

    for (auto table : other.tables) {
        auto * newTable = new Table(*table);
        tables.push_back(newTable);
    }

    for (auto &k : actionsLog) {
        delete k;
    }
    actionsLog.clear();

    for (auto l : other.actionsLog) {
        actionsLog.push_back(l->clone());
    }

    menu.clear();
    for (const auto &m : other.menu) {
        menu.push_back(m);
    }
    return *this;
}

Restaurant &Restaurant::operator=(Restaurant &&other) {
    if (this == &other)
        return *this;
    open = other.open;
    nextCustomerId = other.nextCustomerId;
    //menu.clear();
    menu = std::move(other.menu);
    for (auto &table : tables) {
        delete table;
    }
    tables.clear();
    tables = other.tables;

    for (auto &j : actionsLog) {
        delete j;
    }
    actionsLog.clear();
    actionsLog = other.actionsLog;
    return *this;
}

void Restaurant::start()
{
    std::cout << "Restaurant is now open!\n";
    bool restOpen(true);
    //std::string action;
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    while (restOpen)
    {
        std::string action1;
        std::cout << "Please enter an Action you want to do: \n";
        std::getline(std::cin, action1);
        action1 += " ";
        pos = action1.find(delimiter);
        token = action1.substr(0, pos);
        action1.erase(0, pos + delimiter.length());
        if(token == "open")
            goOpen(action1);
        else if(token == "order")
            goOrder(action1);
        else if(token == "move")
            goMove(action1);
        else if(token == "close")
            goClose(action1);
        else if (token == "closeall") {
            goCloseAll();
            restOpen = false;
        }
        else if (token == "menu")
            goMenu();
        else if (token == "status")
            goStatus(action1);
        else if (token == "log")
            goLog();
        else if (token == "backup")
            goBackup();
        else if (token == "restore")
            goRestore();
        else
            std::cout << "Wrong input, try again \n";
    }
}

int Restaurant::getNumOfTables() const {
    return static_cast<int>(tables.size());
}

Table* Restaurant::getTable(int ind) {
    int tablesSize = static_cast<int>(tables.size());
    if (!(ind > tablesSize || ind < 0))
        return tables[ind];
    return nullptr;
}

std::vector<Dish>& Restaurant::getMenu() {return menu; }

const std::vector<BaseAction*>& Restaurant::getActionsLog() const { return actionsLog;}

DishType Restaurant::findMyType(std::string strType)
{
    if(strType == "ALC")
        return ALC;
    if(strType == "VEG")
        return VEG;
    if(strType == "SPC")
        return SPC;
    else
        return BVG;
}

void Restaurant::goOpen(std::string details)
{
    newMsg = details;
    std::vector<Customer*> customers;
    int tableId;
    std::string delimiterSpace = " ";
    std::string delimiterComa = ",";
    size_t pos1 = 0;
    std::string token1;
    std::string token2;
    pos1 = details.find(delimiterSpace);
    token1 = details.substr(0,pos1);
    tableId = std::stoi(token1);//get the id
    details.erase(0, pos1 + delimiterSpace.length());

    //get the customers
    size_t pos2 = 0;
    while((pos2 = details.find(delimiterSpace)) != std::string::npos && getTable(tableId) != nullptr)
    {
        pos2 = details.find(delimiterComa);
        token1 = details.substr(0, pos2);
        details.erase(0, pos2 + delimiterComa.length());
        pos2 = details.find(delimiterSpace);
        token2 = details.substr(0, pos2);
        details.erase(0, pos2 + delimiterSpace.length());
        if(!token1.empty() && !token2.empty() && !tables.at(static_cast<unsigned long>(tableId))->isOpen())
        {
            Customer *newCustomer = createCustomerByType(token1, token2);
            customers.push_back(newCustomer);
            nextCustomerId++;
            token1="";
            token2="";
        }
    }
    auto open = new OpenTable(static_cast<unsigned long>(tableId), customers);
    open->act(*this);
    actionsLog.push_back(open);
}

void Restaurant::goOrder(std::string details)
{
    int tableId;
    std::string delimiterSpace = " ";
    size_t pos1 = 0;
    std::string token1;
    pos1 = details.find(delimiterSpace);
    token1 = details.substr(0,pos1);
    tableId = std::stoi(token1);//get the id
    details.erase(0, pos1 + delimiterSpace.length());

    auto * ord = new Order(tableId);
    ord->act(*this);
    actionsLog.push_back(ord);
}

void Restaurant::goMove(std::string details)
{
    int customerId;
    int tableId1;
    int tableId2;
    std::string delimiterSpace = " ";
    size_t pos1 = 0;
    std::string token1;
    pos1 = details.find(delimiterSpace);
    token1 = details.substr(0,pos1);
    customerId = std::stoi(token1);
    details.erase(0, pos1 + delimiterSpace.length());


    pos1 = details.find(delimiterSpace);
    token1 = details.substr(0,pos1);
    tableId1 = std::stoi(token1);
    details.erase(0, pos1 + delimiterSpace.length());

    pos1 = details.find(delimiterSpace);
    token1 = details.substr(0, pos1);
    tableId2 = std::stoi(token1);
    details.erase(0, pos1 + delimiterSpace.length());

    auto * newMove = new MoveCustomer(customerId, tableId1, tableId2);
    newMove->act(*this);
    actionsLog.push_back(newMove);

}

void Restaurant::goClose(std::string details)
{
    int tableId;
    std::string delimiterSpace = " ";
    size_t pos1 = 0;
    std::string token1;
    pos1 = details.find(delimiterSpace);
    token1 = details.substr(0,pos1);
    tableId = std::stoi(token1);

    auto * newClose = new Close(tableId);
    newClose->act(*this);
    actionsLog.push_back(newClose);
}

void Restaurant::goCloseAll()
{
    auto * newCloseAll = new CloseAll();
    newCloseAll->act(*this);
    actionsLog.push_back(newCloseAll);
}

void Restaurant::goMenu()
{
    auto * newPrintMenu = new PrintMenu();
    newPrintMenu->act(*this);
    actionsLog.push_back(newPrintMenu);
}

void Restaurant::goStatus(std::string details)
{
    int tableId;
    std::string delimiterSpace = " ";
    size_t pos1 = 0;
    std::string token1;
    pos1 = details.find(delimiterSpace);
    token1 = details.substr(0,pos1);
    tableId = std::stoi(token1);

    auto * newTableStatus = new PrintTableStatus(tableId);
    newTableStatus->act(*this);
    actionsLog.push_back(newTableStatus);
}

void Restaurant::goLog()
{
    auto * newLog = new PrintActionsLog();
    newLog->act(*this);
    actionsLog.push_back(newLog);
}

void Restaurant::goBackup()
{
    auto * newBackup = new BackupRestaurant();
    actionsLog.push_back(newBackup);
    newBackup->act(*this);
}


void Restaurant::goRestore()
{
    auto * newRestore = new RestoreResturant();
    newRestore->act(*this);
    actionsLog.push_back(newRestore);
}

Customer* Restaurant::createCustomerByType(std::string name, std::string strategy)
{
    if (strategy == "veg")
        return new VegetarianCustomer(name, nextCustomerId);
    if (strategy == "spc")
        return new SpicyCustomer(name, nextCustomerId);
    if (strategy == "chp")
        return new CheapCustomer(name, nextCustomerId);
    else
        return new AlchoholicCustomer(name, nextCustomerId);
}

std::string Restaurant::logOpenTableMsg()
{
    return newMsg;
}
