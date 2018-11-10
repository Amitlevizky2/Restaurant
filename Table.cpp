//
// Created by Amit Levizky on 11/3/18.
//

#include "Table.h"
#include <iostream>

Table::Table(int t_capacity) : capacity(t_capacity), open(false) {}

int Table::getCapacity() const {return capacity;}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);//Add Condition
}

void Table::removeCustomer(int id) {
    Customer* temp = getCustomer(id);
    if (temp == nullptr)
        std::cout << "There is no Customer with this id in this table!";
    else

}

Customer* Table::getCustomer(int id) {
    for (int i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id)
            return customersList[i];
        return nullptr;
    }
}

bool Table::isOpen() { return open;}

std::vector<Customer*>& Table::getCustomers() {return customersList;}

std::vector<OrderPair>& Table::getOrders() { return orderList;}

void Table::order(const std::vector<Dish> &menu) {
    for (int i = 0; i < customersList.size(); ++i) {
        Customer* currentCustomer = customersList[i];
        std::vector <int> ordrByCtr = currentCustomer[i].order(menu);
        for (int j = 0; j < ordrByCtr.size(); ++j) {
            orderList.push_back(std::make_pair(currentCustomer[i], menu[ordrByCtr[j]]));
        }
    }
}

void Table::openTable() {
    open = true;
}

void Table::closeTable() {}

int Table::getBill() {}