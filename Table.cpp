//
// Created by Amit Levizky on 11/3/18.
//

#include "Table.h"
#include <iostream>

Table::Table(int t_capacity) : capacity(t_capacity), open(false), customersList(), orderList() {}

Table::Table(const Table &other) :  capacity(other.capacity), open(other.open), customersList(), orderList(other.orderList)
{
    for (auto i : other.customersList) {
        customersList.push_back(i->clone());
    }
}

Table::~Table()
{
    for (auto &i : customersList) {
        delete i;
    }
    customersList.clear();
}

Table &Table::operator=(const Table &other) {
            if(this == &other)
                return *this;
            capacity = other.capacity;
            open = other.open;
            for (auto &i : customersList) {
                delete i;
            }
            customersList.clear();
            for (auto j : other.customersList) {
                customersList.push_back(j->clone());
            }
            orderList = std::move(orderList);
            return *this;
        }

Table::Table(Table &&other) : capacity(other.capacity), open(other.open), customersList(std::move(other.customersList)), orderList(std::move(other.orderList))
{
}
Table &Table::operator=(Table &&other)
        {
            if(this == &other)
                return *this;
            capacity = other.capacity;
            open = other.open;
            for (auto &i : customersList) {
                delete i;
            }
            customersList.clear();
            for (auto j : other.customersList) {
                customersList.push_back(j);
            }
            orderList = std::move(other.orderList);
            return *this;
        }
int Table::getCapacity() const {return capacity;}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Table::removeCustomer(int id) {
    int size = customersList.size();
    for (int i = 0; i < size; ++i) {
        if(customersList.at(i)->getId() == id ) {
            customersList.erase(customersList.begin() + i);
        }

        std::vector<OrderPair> newPairOrderList;
        for (auto &j : orderList) {
            if (j.first != id){
                newPairOrderList.push_back(j);
            }
        }
        orderList = std::move(newPairOrderList);
    }
}

Customer* Table::getCustomer(int id) {
    for (auto &i : customersList) {
        if (i->getId() == id)
            return i;
    }
    return nullptr;
}

std::vector<Customer*>& Table::getCustomers() {return customersList;}

std::vector<OrderPair>& Table::getOrders() { return orderList;}

void Table::order(const std::vector<Dish> &menu) {
    for (auto currentCustomer : customersList) {
        std::vector <int> ordrByCtr = currentCustomer->order(menu);
        for (int j : ordrByCtr) {
            OrderPair newOrdByCtr(currentCustomer->getId(), menu.at(static_cast<unsigned long>(j)));
            orderList.push_back(newOrdByCtr);
            std::cout << currentCustomer->getName() << " ordered " << menu.at(static_cast<unsigned long>(j)).getName() << "\n";
        }
    }
}

void Table::openTable() {
    open = true;
}

void Table::closeTable() {
    for (auto &i : customersList) {
        delete i;
    }
    customersList.clear();
    orderList.clear();
    open=false;
}

int Table::getBill()
{
    int totalBill=0;
    for (auto &i : orderList) {
        totalBill = totalBill + i.second.getPrice();
    }
    return totalBill;
}

bool Table::isOpen() { return open;}