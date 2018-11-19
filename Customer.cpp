#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

//
// Created by Amit Levizky on 11/3/18.
//

#include "Customer.h"
#include <algorithm>
//Base Customer class
Customer::Customer(std::string c_name, int c_id) : name(std::move(c_name)), id(c_id) {
}



std::string Customer::getName() const { return name;}

int Customer::getId() const { return id;}

//End Base Customer class

//VegetarianCustomer class

VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(std::move(name), id), mstExpBvg(-1), smllstIdVeg(1000), findAndLeg(false) {
}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    if (!findAndLeg) {
        for (const auto &i : menu) {
            if (i.getType() == VEG && i.getId() < smllstIdVeg)
                smllstIdVeg = i.getId();
            if (i.getType() == BVG && i.getPrice() > mstExpBvg)
                mstExpBvg = i.getId();
        }
    }
    if (mstExpBvg >= 0 && mstExpBvg <= 1000 &&  smllstIdVeg >= 0 && smllstIdVeg <= 1000)
    {
        findAndLeg = true;
        std::vector<int> newOrd;
        newOrd.push_back(smllstIdVeg);
        newOrd.push_back(mstExpBvg);
        return newOrd;
    }

    return std::vector<int>();
}

std::string VegetarianCustomer::toString() const
{
    return getName() + ",veg ";
}

Customer *VegetarianCustomer::clone()
{
    return new VegetarianCustomer(*this);
}

//End of VegetarianCustomer class


//CheapCustomer class
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(std::move(name), id), ordered(false), cheapest(INT8_MAX){
}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> newOrd;
    if(!ordered && !menu.empty())
    {
        int size = menu.size();
        cheapest = 0;
        for (int i = 0; i < size; ++i)
        {
            if(menu.at(static_cast<unsigned long>(cheapest)).getPrice() > menu.at(static_cast<unsigned long>(i)).getPrice())
                cheapest = i;
        }
    }
    if (cheapest != INT8_MAX && !ordered)
    {
        newOrd.push_back(cheapest);
    }
    ordered = true;
    return newOrd;
}

std::string CheapCustomer::toString() const {return getName() + ",chp ";}

Customer *CheapCustomer::clone() {
    return new CheapCustomer(*this);
}
//End of CheapCustomer class

//SpicyCustomer class
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(std::move(name), id), spcMstExpInx(INT8_MAX), spcPrice(-1), bvgMstChpstInx(INT8_MAX), bvgPrice(INT8_MAX), ordered(false){
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> newOrd;
    if (!ordered) {
        int size = menu.size();
        for (int i = 0; i < size; ++i) {
            if (menu[i].getType() == SPC && menu[i].getPrice() >= spcPrice) {
                spcPrice = menu[i].getPrice();
                spcMstExpInx = i;
            }

            if (menu[i].getType() == BVG && menu[i].getPrice() <= bvgPrice) {
                bvgPrice = menu[i].getPrice();
                bvgMstChpstInx = i;
            }

        }
    }
    if (!ordered && spcMstExpInx < INT8_MAX)
    {
        newOrd.push_back(spcMstExpInx);
        ordered = true;
        return  newOrd;
    }
    else if(ordered && bvgMstChpstInx < INT8_MAX)
    {
        newOrd.push_back(bvgMstChpstInx);
        return newOrd;
    }
    return std::vector<int>();

}

std::string SpicyCustomer::toString() const {return getName() + ",spc ";}

Customer *SpicyCustomer::clone() {
    return new SpicyCustomer(*this);
}


// AlchoholicCustomer class
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(std::move(name), id) , maxAlc(0), maxInx(0) ,minAlc(INT8_MAX),  minInx(INT8_MAX), ordered(false){}

std::vector<int>  AlchoholicCustomer::order(const std::vector<Dish> &menu)
{
    int lastChecked = INT8_MAX;
    int lastCheckedIndex = INT8_MAX;
    std::vector<int> newOrder;
    int size = menu.size();
    if(minInx == maxInx)
        return newOrder;
    if(!ordered)
    {
        for (int i = 0; i < size; ++i)
        {
            if (menu.at(static_cast<unsigned long>(i)).getType() == ALC && menu.at(
                    static_cast<unsigned long>(i)).getPrice() < minAlc)
            {
                minAlc = menu.at(static_cast<unsigned long>(i)).getPrice();
                minInx = i;
            }

            if(menu.at(static_cast<unsigned long>(i)).getType() == ALC && menu.at(
                    static_cast<unsigned long>(i)).getPrice() >= maxAlc)
            {
                maxAlc = menu.at(static_cast<unsigned long>(i)).getPrice();
                maxInx = i;
            }
        }

        if(minAlc < INT8_MAX)
        {
            newOrder.push_back(minInx);
            ordered = true;
            return newOrder;
        }
        ordered = true;
        return newOrder;
    }
    for (int i = 0; i < size; ++i)
    {
        if(menu.at(static_cast<unsigned long>(i)).getPrice() >= minAlc && menu.at(
                static_cast<unsigned long>(i)).getType() == ALC)
        {
            if(menu.at(static_cast<unsigned long>(i)).getPrice() == minAlc && minInx < i)// && !changed
            {
                minInx = i;
                minAlc = menu.at(static_cast<unsigned long>(i)).getPrice();
                newOrder.push_back(minInx);
                return newOrder;
            }
            else if(menu.at(static_cast<unsigned long>(i)).getPrice() > minAlc && menu.at(
                    static_cast<unsigned long>(i)).getPrice() < lastChecked)
            {
                lastChecked = menu.at(static_cast<unsigned long>(i)).getPrice();
                lastCheckedIndex = i;
            }
        }
    }
    minInx = lastCheckedIndex;
    minAlc = lastChecked;
    if(minInx != INT8_MAX)
        newOrder.push_back(minInx);
    return newOrder;
}



std::string AlchoholicCustomer::toString() const {return getName() + ",alc ";}

Customer *AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}
