//
// Created by Amit Levizky on 11/3/18.
//

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual  ~Customer();
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* clone() = 0;

private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    ~VegetarianCustomer();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
private:
    int mstExpBvg;
    int smllstIdVeg;
    bool findAndLeg;
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    ~CheapCustomer();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
private:
    bool ordered;
    int cheapest;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    ~SpicyCustomer();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
private:
    int spcMstExpInx;
    int spcPrice;
    int bvgMstChpstInx;
    int bvgPrice;
    bool ordered;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    ~AlchoholicCustomer();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* clone();
private:

};


#endif //CUSTOMER_H_
