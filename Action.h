//
// Created by Amit Levizky on 11/3/18.
//

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    BaseAction(const BaseAction& other);
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() = 0;
    virtual  ~BaseAction() = default;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    OpenTable(const OpenTable& other);
    virtual ~OpenTable() = default;
    OpenTable(OpenTable&& other);
    BaseAction *clone() override;
    void act(Restaurant &restaurant) override;
    std::string toString() const override;
private:
    //std::string customersString();
    const int tableId;
    std::vector<Customer *> customers;
    std::string completeMessage;
};


class Order : public BaseAction {
public:
    Order(int id);
    virtual  ~Order() = default;
    BaseAction *clone() override;
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    virtual  ~MoveCustomer() = default;
    void act(Restaurant &restaurant);
    BaseAction *clone() override;
    std::string toString() const;
private:
    std::vector <int> getCustomerOrderList(int custId, int tableId, Restaurant &restaurant);
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    virtual  ~Close() = default;
    BaseAction *clone() override;
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    virtual  ~CloseAll() = default;
    BaseAction *clone() override;
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    virtual  ~PrintMenu() = default;
    BaseAction *clone() override;
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    std::vector<Dish> menu;
    std::string findMyType(DishType type);
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    virtual  ~PrintTableStatus() = default;
    BaseAction *clone() override;
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual  ~PrintActionsLog() = default;
    BaseAction *clone() override;
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    virtual  ~BackupRestaurant() = default;
    BaseAction *clone() override;
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    virtual  ~RestoreResturant() = default;
    BaseAction *clone() override;
    void act(Restaurant &restaurant);
    std::string toString() const;

};

#endif //ACTION_H_
