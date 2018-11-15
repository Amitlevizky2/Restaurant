//
// Created by Amit Levizky on 11/3/18.
//
#include "Restaurant.h"
#include <vector>
#include <iostream>
#include <fstream>

Restaurant::Restaurant() : open(true) {}

Restaurant::Restaurant(const std::string &configFilePath) :  open(true)
{
    std::ifstream myfile(configFilePath);
    std::string line;
    std::string delimeter = ",";
    int numOfTables = INT8_MAX;

    if(myfile.is_open())
    {
        while (getline(myfile, line))
        {
            line = line + ",";
            size_t pos = 0;
            std::string token;
            std::vector<std::string> newDish;
            while ((pos = line.find(delimeter)) != std::string::npos && line[0] != '#')
            {
                token = line.substr(0, pos);
                if(numOfTables == INT8_MAX)
                {
                    numOfTables = std::stoi(token);
                    line.erase(0, pos + delimeter.length());
                }
                else if(tables.size() < numOfTables)
                {
                    Table *t = new Table(std::stoi(token));
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
                    Dish toBeAdd(menu.size(), newDish[0], std::stoi(newDish[2]), menuDish);
                    menu.push_back(toBeAdd);
                    newDish.clear();
                }
            }
        }
    }
}

Restaurant::Restaurant(const Restaurant &other) :open(true), menu(other.menu)
{
    for (int i = 0; i < other.tables.size(); ++i)
    {
        Table* tempTable = new Table(other.tables[i]->getCapacity());
        tables.push_back(tempTable);
    }
    for (int j = 0; j < other.actionsLog.size(); ++j)
    {
        BaseAction* tempBaseAction = other.actionsLog[j]->clone();
        actionsLog.push_back(tempBaseAction);
    }
}

Restaurant::~Restaurant()
{
    for (int i = 0; i < tables.size(); ++i)
    {
        delete tables[i];
    }
    for (int j = 0; j < actionsLog.size(); ++j)
    {
        delete(actionsLog[j]);
    }
}

Restaurant::Restaurant(Restaurant &&other) : tables(std::move(other.tables)), actionsLog(std::move(other.actionsLog)), menu(std::move(other.menu))
{
}//TODO::to be complete

Restaurant &Restaurant::operator=(const Restaurant &other) {
    if (this == &other)
        return *this;
    open = other.open;
    for (int i = 0; i < other.tables.size(); ++i)
    {
        delete tables[i];
    }
    tables.clear();

    for (int j = 0; j < other.tables.size(); ++j)
    {
        tables.push_back(other.tables[j]);
    }

    for (int k = 0; k < other.actionsLog.size(); ++k)
    {
        delete actionsLog[k];
    }
    actionsLog.clear();

    for (int l = 0; l < other.actionsLog.size(); ++l)
    {
        actionsLog.push_back(other.actionsLog[l]);
    }
}

Restaurant &Restaurant::operator=(Restaurant &&other) {
    if (this == &other)
        return *this;
    open = other.open;
    //menu.clear();
    menu = std::move(other.menu);
    for (int i = 0; i < tables.size(); ++i)
    {
        delete tables[i];
    }
    tables.clear();
    tables = other.tables;

    for (int j = 0; j < actionsLog.size(); ++j)
    {
        delete actionsLog[j];
    }
    actionsLog.clear();
    actionsLog = other.actionsLog;
    return *this;
}

void Restaurant::start()
{

}

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

DishType Restaurant::findMyType(std::string strType)
{
    if(strType == "ALC")
        return ALC;
    if(strType == "VEG")
        return VEG;
    if(strType == "SPC")
        return SPC;
    if(strType == "BVG")
        return BVG;
}
