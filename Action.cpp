//
// Created by Amit Levizky on 11/3/18.
//

#include "Action.h"
#include "Table.h"

//Start of BaseAction Abstract class
BaseAction::BaseAction() {
    status = PENDING;
}

ActionStatus BaseAction::getStatus() const {return status;}

void BaseAction::complete() {status = COMPLETED; }

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg << std::endl;

}

std::string BaseAction::getErrorMsg() const {return errorMsg;}

//End of BaseAction


//OpenTable Action
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id), customers(customersList), BaseAction() {
}

void OpenTable::act(Restaurant &restaurant) {
    Table* temp = restaurant.getTable(tableId);
    if(temp == nullptr || temp->isOpen())
    {
        error("Table does not exist or is already open");
    }
    else
    {
        //temp->openTable();
        temp->getCustomers() = customers;
        complete();
    }
}

std::string OpenTable::toString() const {}
//End of OpenTable class

Order::Order(int id) : tableId(id), BaseAction() {
}

void Order::act(Restaurant &restaurant) {
    Table* currentTable = restaurant.getTable(tableId);
    if(currentTable == nullptr || !(currentTable->isOpen()))
    {
        error("Table does not exist or is not open");
    }
    else
    {
        currentTable->order(restaurant.getMenu());
        complete();
    }
}

std::string Order::toString() const {}
//End Order class







