//
// Created by Amit Levizky on 11/3/18.
//

#include "Action.h"
#include "Table.h"

//Start of BaseAction Abstract class
BaseAction::BaseAction():status(PENDING), errorMsg("") {
    status = PENDING;
}

BaseAction::BaseAction(const BaseAction& other) : status(PENDING), errorMsg("") {}

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
//constructor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id), customers(customersList), BaseAction() {
}

//copy constructor
OpenTable::OpenTable(const OpenTable& other) : BaseAction(other), tableId(other.tableId)
{
    for (int i = 0; i < other.customers.size(); ++i)
    {
        customers.push_back(other.customers[i]->clone());
    }
}

//destructor
OpenTable::~OpenTable()
{
    for (int i = 0; i < customers.size(); ++i)
    {
        delete customers[i];
    }
}

//Move constructor
OpenTable::OpenTable(OpenTable&& other) : BaseAction(), tableId(other.tableId), customers(std::move(other.customers))
{
}

void OpenTable::act(Restaurant &restaurant) {
    Table* temp = restaurant.getTable(tableId);
    if(temp == nullptr || temp->isOpen())
    {
        error("Table does not exist or is already open");
    }
    else
    {
        temp->openTable();
        temp->getCustomers() = customers;
        complete();
    }
}



std::string OpenTable::toString() const {}



//End of OpenTable class


//Order class
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


//MoveCustomer class
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src), dstTable(dst), id(customerId),BaseAction() {}

void MoveCustomer::act(Restaurant &restaurant) {
    Table* source = restaurant.getTable(srcTable);
    Table* destination = restaurant.getTable(dstTable);

    if (source == nullptr || destination == nullptr || !source->isOpen() || !destination->isOpen() || source->getCustomer(id) == nullptr || destination->getCapacity() == destination->getCustomers().size())
        error("Cannot move customer");
    else
    {
        std::vector<int> customerOrders = getCustomerOrderList(id, srcTable, restaurant);//Get the orders of the customer from the source table
        for (int i = 0; i < customerOrders.size(); ++i) {
            destination->getOrders().push_back(id, restaurant.getMenu()[customerOrders[i]]);
        }
        destination->getCustomers().push_back(source->getCustomer(id));//Move the customer to the destination table
        source->removeCustomer(id);//Remove the customer from the source table
        std::cout << "move " << srcTable << " " << dstTable << " " << id;
        if(source->getCustomers().empty())
            source->closeTable();
        complete();
    }

}

std::string MoveCustomer::toString() const {}//TODO:toString Method


//Get the orders of the customer from the source table
std::vector <int> MoveCustomer::getCustomerOrderList(int custId, int tableId, Restaurant &restaurant) {
    Table* table = restaurant.getTable(tableId);
    Customer* customer = table->getCustomer(custId);
    std::vector<OrderPair> customerPairOrder = table->getOrders();
    std::vector<int> customerOrders;
    for (int i = 0; i < customerPairOrder.size(); ++i) {
        if (custId == customerPairOrder[i].first)
            customerOrders.push_back(customerPairOrder[i].second.getId());
    }
    return customerOrders;

}
//End of MoveCustomer class



//Close class
Close::Close(int id) : tableId(id), BaseAction() {}

void Close::act(Restaurant &restaurant) {
    Table* table = restaurant.getTable(id);
    if(table == nullptr || !table->isOpen()) {
        error("Table does not exist or is not open");
    }
    
    else {
        std::cout << "Table " << tableId << " was closed. Bill " << table->getBill() << "NIS";
        table->closeTable();
    }
}

std::string Close::toString() const {}
//End of Close class


//CloseAll class
CloseAll::CloseAll() : BaseAction() {}

void CloseAll::act(Restaurant &restaurant)
{
    for (int i = 0; i < restaurant.getNumOfTables(); ++i)
    {
        Table* table = restaurant.getTable(i);
        if(table != nullptr) {
            std::cout << "Table " << i << " was closed. Bill " << table->getBill() << "NIS";
            table.closeTable();
        }
    }
    complete();
}

std::string CloseAll::toString() const {}//TODO:toString()
//End of CloseAll class


//PrintMenu class
PrintMenu::PrintMenu() : BaseAction() {}

void PrintMenu::act(Restaurant &restaurant)
{
    menu = restaurant.getMenu();
    for (int i = 0; i < menu.size(); ++i)
    {
        std::cout << menu[i].getName() << " " << menu[i].getType() << " " << menu[i].getPrice() << "NIS";
    }
    complete();
}

std::string PrintMenu::toString() const {}//TODO:toString()
//End of PrintMenu class


//PrintTableStatus class
PrintTableStatus::PrintTableStatus(int id) : tableId(id), BaseAction() {}

void PrintTableStatus::act(Restaurant &restaurant) {}

std::string PrintTableStatus::toString() const {}
//End of PrintTableStatus class


//PrintActionsLog class
PrintActionsLog::PrintActionsLog() : BaseAction() {}

void PrintActionsLog::act(Restaurant &restaurant) {}

std::string PrintActionsLog::toString() const {}
//End of PrintMenu class


//BackupResturant class
BackupRestaurant::BackupRestaurant() : BaseAction() {}

void BackupRestaurant::act(Restaurant &restaurant) {}

std::string BackupRestaurant::toString() const {}
//End of BackupRestaurant class


//RestoreResturant class
RestoreResturant::RestoreResturant() : BaseAction() {}

void RestoreResturant::act(Restaurant &restaurant) {}

std::string RestoreResturant::toString() const {}
//End of RestoreResturant class













