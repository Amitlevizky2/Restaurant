#include <utility>

//
// Created by Amit Levizky on 11/3/18.
//

#include "Action.h"
#include "Table.h"
#include "Restaurant.h"
extern Restaurant* backup;
//Start of BaseAction Abstract class
BaseAction::BaseAction():errorMsg(""), status(PENDING) {
}

ActionStatus BaseAction::getStatus() const {return status;}

void BaseAction::complete() {status = COMPLETED; }

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = std::move(errorMsg);
    //std::cout << errorMsg << std::endl;

}

std::string BaseAction::getErrorMsg() const {return errorMsg;}

//End of BaseAction


//OpenTable Action
//constructor
OpenTable::OpenTable(unsigned long id, std::vector<Customer *> &customersList) : BaseAction(), tableId(id), customers(customersList), completeMessage(std::move("")) {
}



void OpenTable::act(Restaurant &restaurant) {
    Table* temp = restaurant.getTable(tableId);
    if(temp == nullptr || temp->isOpen())
    {
        std::cout << "Table does not exist or is already open\n";
        error("open " + restaurant.logOpenTableMsg() + "Error: Table does not exist or is already open");
    }
    else
    {
        temp->openTable();
        temp->getCustomers() = customers;
        complete();
    }

    if(getStatus() == ActionStatus::COMPLETED)
    {

        completeMessage = "open " + std::to_string(tableId) + " " ;
        for (auto &customer : customers) {
            completeMessage.append(customer->toString());
        }
        completeMessage.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        completeMessage.append(getErrorMsg());
    }
    else completeMessage.append("Pending");

}




std::string OpenTable::toString() const
{
    return completeMessage;
}

BaseAction *OpenTable::clone() {
    return (new OpenTable(*this));
}




//End of OpenTable class


//Order class
Order::Order(int id) :  BaseAction(), tableId(id) {
}

void Order::act(Restaurant &restaurant) {
    Table* currentTable = restaurant.getTable(tableId);
    if(currentTable == nullptr || !(currentTable->isOpen()))
    {
        std::cout << "Table does not exist or is not open\n";
        error(" Table does not exist or is not open");
    }
    else
    {
        currentTable->order(restaurant.getMenu());
        complete();
    }
}




std::string Order::toString() const
{
    std::string msg("order " + std::to_string(tableId) + " ");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *Order::clone() {
    return (new Order(*this));
}

//End Order class


//MoveCustomer class
MoveCustomer::MoveCustomer(int src, int dst, int customerId) :BaseAction(), srcTable(src), dstTable(dst), id(customerId) {}

void MoveCustomer::act(Restaurant &restaurant) {
    Table* source = restaurant.getTable(srcTable);
    Table* destination = restaurant.getTable(dstTable);
    int size = destination->getCustomers().size();
    if (source == nullptr || destination == nullptr || !source->isOpen() || !destination->isOpen() || source->getCustomer(id) == nullptr || destination->getCapacity() == size)
    {
        std::cout << "Cannot move customer\n";
        error("Cannot move customer");
    }
    else
    {
        std::vector<int> customerOrders = getCustomerOrderList(id, srcTable, restaurant);//Get the orders of the customer from the source table
        for (int customerOrder : customerOrders) {
            OrderPair a(id,restaurant.getMenu()[customerOrder]);
            destination->getOrders().push_back(a);
            //destination->getBill() = (destination->getBill() + a.second.getPrice());
        }
        destination->getCustomers().push_back(source->getCustomer(id));//Move the customer to the destination table
        source->removeCustomer(id);//Remove the customer from the source table
        if(source->getCustomers().empty())
            source->closeTable();
        complete();
    }

}

std::string MoveCustomer::toString() const
{
    std::string msg("move " + std::to_string(srcTable) +" " + std::to_string(dstTable) + " " + std::to_string(id) + " ");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *MoveCustomer::clone() {
    return (new MoveCustomer(*this));
}


//Get the orders of the customer from the source table
std::vector <int> MoveCustomer::getCustomerOrderList(int custId, int tableId, Restaurant &restaurant) {
    Table* table = restaurant.getTable(tableId);
    std::vector<OrderPair> customerPairOrder = table->getOrders();
    std::vector<int> customerOrders;
    for (auto &i : customerPairOrder) {
        if (custId == i.first)
            customerOrders.push_back(i.second.getId());
    }
    return customerOrders;

}
//End of MoveCustomer class



//Close class
Close::Close(int id) : BaseAction(), tableId(id) {}

void Close::act(Restaurant &restaurant) {
    Table* table = restaurant.getTable(tableId);
    if(table == nullptr || !table->isOpen()) {
        std::cout << "Table does not exist or is not open\n";
        error("Table does not exist or is not open");
    }
    
    else {
        std::cout << "Table " << tableId << " was closed. Bill " << table->getBill() << "NIS\n";
        table->closeTable();
        complete();
    }
}

std::string Close::toString() const
{
    std::string msg("close " + std::to_string(tableId) + " ");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *Close::clone() {
    return (new Close(*this));
}
//End of Close class


//CloseAll class
CloseAll::CloseAll() : BaseAction() {}

void CloseAll::act(Restaurant &restaurant)
{
    for (int i = 0; i < restaurant.getNumOfTables(); ++i)
    {
        Table* table = restaurant.getTable(i);
        if(table != nullptr && table->isOpen()) {
            std::cout << "Table " << i << " was closed. Bill " << table->getBill() << "NIS\n";
        }
    }
    complete();
}

std::string CloseAll::toString() const
{
    std::string msg("Closeall");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *CloseAll::clone() {
    return (new CloseAll(*this));
}
//End of CloseAll class


//PrintMenu class
PrintMenu::PrintMenu() : BaseAction() {}

void PrintMenu::act(Restaurant &restaurant)
{
    for (auto &i : restaurant.getMenu()) {
        std::cout << i.getName() << " " << findMyType(i.getType()) << " " << i.getPrice() << "NIS \n";
    }
    complete();
}

std::string PrintMenu::toString() const
{
    std::string msg("menu ");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

std::string PrintMenu::findMyType(DishType type)
{
    if (type == ALC)
        return "ALC";
    if(type == BVG)
        return "BVG";
    if (type == SPC)
        return "SPC";
    return "VEG";
}

BaseAction *PrintMenu::clone() {
    return (new PrintMenu(*this));
}
//End of PrintMenu class


//PrintTableStatus class
PrintTableStatus::PrintTableStatus(int id) : BaseAction(), tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant)
{
    Table* table = restaurant.getTable(tableId);
    std::vector<OrderPair> customerPairOrder = table->getOrders();
    if(!table->isOpen())
        std::cout << "Table " << tableId << " status: closed\n";
    else {
        std::cout << "Table " << tableId << " status: open\n";
        std::cout << "Customers: \n";
        for (auto &i : table->getCustomers()) {
            std::cout << i->getId() << " " << i->getName() << "\n";
        }
        std::cout << "Orders: \n";
        for (auto &i : customerPairOrder) {
            std::cout << i.second.getName() << " " << i.second.getPrice() << "NIS "
                      << i.first << " \n";
        }
        std::cout << "Current Bill: " << table->getBill() << "NIS\n";
    }
    complete();

}

std::string PrintTableStatus::toString() const
{
    std::string msg("status " + std::to_string(tableId) + " ");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *PrintTableStatus::clone() {
    return (new PrintTableStatus(*this));
}

//End of PrintTableStatus class


//PrintActionsLog class
PrintActionsLog::PrintActionsLog() : BaseAction() {}

void PrintActionsLog::act(Restaurant &restaurant)
{
    std::vector<BaseAction*>res = restaurant.getActionsLog();
    for (auto i : res)
    {
        std::cout << i->toString() << "\n" ;
    }
    complete();
}

std::string PrintActionsLog::toString() const
{
    std::string msg("log ");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *PrintActionsLog::clone() {
    return (new PrintActionsLog(*this));
}
//End of PrintMenu class


//BackupResturant class
BackupRestaurant::BackupRestaurant() : BaseAction() {}

void BackupRestaurant::act(Restaurant &restaurant)
{
    complete();
    if(backup!= nullptr)
    {
        delete backup;
        backup= nullptr;
        backup =  new Restaurant(restaurant);
    }
    else
        backup = new Restaurant(restaurant);
}

std::string BackupRestaurant::toString() const
{
    std::string msg("backup ");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *BackupRestaurant::clone() {
    return (new BackupRestaurant(*this));
}
//End of BackupRestaurant class


//RestoreResturant class
RestoreResturant::RestoreResturant() : BaseAction() {}

void RestoreResturant::act(Restaurant &restaurant)
{
    if(backup != nullptr)
    {
        restaurant = *backup;
        complete();
    }
    else
    {
        std::cout << "No Backup available\n";
        error("No Backup available");
    }

}

std::string RestoreResturant::toString() const
{
    std::string msg("restore ");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append("Error:" + getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *RestoreResturant::clone() {
    return (new RestoreResturant(*this));
}
//End of RestoreResturant class













