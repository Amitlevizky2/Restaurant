//
// Created by Amit Levizky on 11/3/18.
//

#include "Table.h"
#include <iostream>

Table::Table(int t_capacity) : capacity(t_capacity), open(false), customersList(), orderList() {}

Table::Table(const Table &other) :  capacity(other.capacity), open(other.open), customersList(), orderList(other.orderList)
{
    for (int i = 0; i < other.customersList.size(); ++i)
    {
        customersList.push_back(other.customersList.at(i)->clone());
    }
}

Table::~Table()
{
    for (int i = 0; i < customersList.size(); ++i)
    {
        delete customersList.at(i);
    }
    customersList.clear();
}

Table &Table::operator=(const Table &other) {
            if(this == &other)
                return *this;
            capacity = other.capacity;
            open = other.open;
            for (int i = 0; i < customersList.size(); ++i)
            {
                delete customersList.at(i);
            }
            customersList.clear();
            for (int j = 0; j < other.customersList.size(); ++j)
            {
                customersList.push_back(other.customersList.at(j)->clone());
            }
            orderList = std::move(orderList);

        }

Table::Table(Table &&other) : capacity(other.capacity), open(other.open), customersList(std::move(other.customersList))
{
}
Table &Table::operator=(Table &&other)
        {
            if(this == &other)
                return *this;
            capacity = other.capacity;
            open = other.open;
            for (int i = 0; i < customersList.size(); ++i)
            {
                delete customersList.at(i);
            }
            customersList.clear();
            for (int j = 0; j < other.customersList.size(); ++j)
            {
                customersList.push_back(other.customersList.at(j));
            }
            orderList = std::move(other.orderList);
        }
int Table::getCapacity() const {return capacity;}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Table::removeCustomer(int id) {
    for (int i = 0; i < customersList.size(); ++i) {
        if(customersList.at(i)->getId() == id ) {
            customersList.erase(customersList.begin() + i);
        }

        std::vector<OrderPair> newPairOrderList;
        for (int j = 0; j < orderList.size(); ++j) {
            if (orderList.at(j).first != id){
                newPairOrderList.push_back(orderList.at(j));
            }
        }
        orderList = std::move(newPairOrderList);
    }
}

Customer* Table::getCustomer(int id) {
    for (int i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id)
            return customersList[i];
    }
    return nullptr;
}

std::vector<Customer*>& Table::getCustomers() {return customersList;}

std::vector<OrderPair>& Table::getOrders() { return orderList;}

void Table::order(const std::vector<Dish> &menu) {
    for (int i = 0; i < customersList.size(); ++i) {
        Customer* currentCustomer = customersList.at(i);
        std::vector <int> ordrByCtr = currentCustomer->order(menu);
        for (int j = 0; j < ordrByCtr.size(); ++j) {
            OrderPair newOrdByCtr(currentCustomer->getId(), menu.at(ordrByCtr.at(j)));
            orderList.push_back(newOrdByCtr);
            std::cout << currentCustomer->getName() << " ordered " << menu.at(ordrByCtr.at(j)).getName() << "\n";
        }
    }
}

void Table::openTable() {
    open = true;
}

void Table::closeTable() {
    for (int i = 0; i < customersList.size(); ++i) {
        delete customersList[i];
    }
    customersList.clear();
    orderList.clear();
    open=false;
}

int Table::getBill()
{
    int totalBill=0;
    for (int i = 0; i < orderList.size(); ++i)
    {
        totalBill = totalBill + orderList.at(i).second.getPrice();
    }
    return totalBill;
}

bool Table::isOpen() { return open;}