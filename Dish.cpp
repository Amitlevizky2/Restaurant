//
// Created by Amit Levizky on 11/3/18.
//

#include "Dish.h"

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), name(d_name), type(d_type), price(d_price){}

int Dish::getId() const {return id;}

std::string Dish::getName() const {return name;}

int Dish::getPrice() const {return price;}

DishType Dish::getType() const {return type;}