//
// Created by Amit Levizky on 11/3/18.
//

#include "Action.h"
#include "Table.h"
#include "Restaurant.h"
extern Restaurant* backup;
//Start of BaseAction Abstract class
BaseAction::BaseAction():status(PENDING), errorMsg("") {
    status = PENDING;
}

BaseAction::BaseAction(const BaseAction& other) : status(PENDING), errorMsg("") {}

ActionStatus BaseAction::getStatus() const {return status;}

void BaseAction::complete() {status = COMPLETED; }

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = " " + errorMsg;
    std::cout << " " << errorMsg << std::endl;

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



//Move constructor
OpenTable::OpenTable(OpenTable&& other) : BaseAction(), tableId(other.tableId), customers(std::move(other.customers))
{
}

void OpenTable::act(Restaurant &restaurant) {
    Table* temp = restaurant.getTable(tableId);
    if(temp == nullptr || temp->isOpen())
    {
        error("open " + restaurant.logOpenTableMsg() + " Error: Table does not exist or is already open");
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
        for (int i=0;i<customers.size();++i)
        {
            completeMessage.append(customers[i]->toString());
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
Order::Order(int id) : tableId(id), BaseAction() {
}

void Order::act(Restaurant &restaurant) {
    Table* currentTable = restaurant.getTable(tableId);
    if(currentTable == nullptr || !(currentTable->isOpen()))
    {
        error("Error: Table does not exist or is not open");
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
        msg.append(getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *Order::clone() {
    return (new Order(*this));
}

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
            OrderPair a(id,restaurant.getMenu()[customerOrders.at(i)]);
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
        msg.append(getErrorMsg());
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
    Table* table = restaurant.getTable(tableId);
    if(table == nullptr || !table->isOpen()) {
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
        msg.append(getErrorMsg());
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
        msg.append(getErrorMsg());
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
    for (int i = 0; i < restaurant.getMenu().size(); ++i)
    {
        std::cout << restaurant.getMenu()[i].getName() << " " << findMyType(restaurant.getMenu()[i].getType()) << " " << restaurant.getMenu()[i].getPrice() << "NIS \n";
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
        msg.append(getErrorMsg());
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
PrintTableStatus::PrintTableStatus(int id) : tableId(id), BaseAction() {}

void PrintTableStatus::act(Restaurant &restaurant)
{
    int sum = 0;
    Table* table = restaurant.getTable(tableId);
    std::vector<OrderPair> customerPairOrder = table->getOrders();
    if(!table->isOpen())
        std::cout << " Table " << tableId << " status: closed\n";
    else {
        std::cout << " Table " << tableId << " status: open\n";
        std::cout << "Customers: \n";
        for (int i = 0; i < table->getCustomers().size(); i++) {
            std::cout << table->getCustomers()[i]->getId() << " " << table->getCustomers()[i]->getName() << "\n";
        }
        std::cout << "Orders: \n";
        for (int i = 0; i < customerPairOrder.size(); i++)
        {
            std::cout << customerPairOrder[i].second.getName() << " " <<customerPairOrder[i].second.getPrice() << "NIS "
                      << customerPairOrder[i].first << " \n";
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
        msg.append(getErrorMsg());
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
        msg.append(getErrorMsg());
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
    if(backup!= nullptr)
    {
        delete backup;
        backup= nullptr;
        *backup = restaurant;
    }
    else
        backup= new Restaurant(restaurant);
    complete();
}

std::string BackupRestaurant::toString() const
{
    std::string msg("backup");
    if(getStatus() == ActionStatus::COMPLETED)
    {
        msg.append("Completed");
    }
    else  if(getStatus() == ActionStatus::ERROR)
    {
        msg.append(getErrorMsg());
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
        msg.append(getErrorMsg());
    }
    else msg.append("Pending");
    return msg;
}

BaseAction *RestoreResturant::clone() {
    return (new RestoreResturant(*this));
}
//End of RestoreResturant class













