//
// Created by Amit Levizky on 11/3/18.
//

#include "Customer.h"
#include <algorithm>
//Base Customer class
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {
}



std::string Customer::getName() const { return name;}

int Customer::getId() const { return id;}

//End Base Customer class

//VegetarianCustomer class

VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id), mstExpBvg(-1), smllstIdVeg(1000), findAndLeg(false) {
}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    if (!findAndLeg) {
        for (int i = 0; i < menu.size(); ++i) {
            if (menu[i].getType() == VEG && menu[i].getId() < smllstIdVeg)
                smllstIdVeg = menu[i].getId();
            if (menu[i].getType() == BVG && menu[i].getPrice() > mstExpBvg)
                mstExpBvg = menu[i].getId();
        }
    }
    if (mstExpBvg >= 0 && mstExpBvg <= 1000 &&  smllstIdVeg >= 0 && smllstIdVeg <= 1000)
    {
        findAndLeg = true;
        return std::vector<int> (smllstIdVeg, mstExpBvg);
    }

    return std::vector<int>();
}

std::string VegetarianCustomer::toString() const {return "";}//TODO: toString

Customer *VegetarianCustomer::clone()
{
    return new VegetarianCustomer(*this);
}

//End of VegetarianCustomer class


//CheapCustomer class
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id), ordered(false), cheapest(INT8_MAX){
}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    if (!ordered)
    {
        if (!menu.empty())
            cheapest = menu[0].getPrice();
        for (int i = 0; i < menu.size(); ++i)
        {
            if (menu[i].getPrice() < menu[cheapest].getPrice())
                cheapest = menu[i].getId();
        }
        ordered =  true;
    }

    if (menu[cheapest].getId() < INT8_MAX)
        return std::vector<int>(cheapest);
    return std::vector<int>();
}

std::string CheapCustomer::toString() const {return "";}//TODO: toString

Customer *CheapCustomer::clone() {
    return new CheapCustomer(*this);
}
//End of CheapCustomer class

//SpicyCustomer class
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id), spcMstExpInx(INT8_MAX), spcPrice(-1), bvgMstChpstInx(INT8_MAX), bvgPrice(INT8_MAX), ordered(false){
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    if (!ordered) {
        for (int i = 0; i < menu.size(); ++i) {
            if (menu[i].getType() == SPC && menu[i].getPrice() >= spcPrice) {
                spcPrice = menu[i].getPrice();
                spcMstExpInx = i;
            }

            if (menu[i].getType() == BVG && menu[i].getPrice() <= bvgPrice) {
                bvgPrice = menu[i].getPrice();
                bvgMstChpstInx = i;
            }

        }
        ordered = true;
    }
    if(spcMstExpInx < INT8_MAX && bvgMstChpstInx < INT8_MAX)
        return std::vector<int>(spcMstExpInx, bvgMstChpstInx);
    else if (spcMstExpInx < INT8_MAX && bvgMstChpstInx == INT8_MAX)
        return std::vector<int>(spcMstExpInx);
    else
        return std::vector<int>();

}

std::string SpicyCustomer::toString() const {return "";}//TODO: toString

Customer *SpicyCustomer::clone() {
    return new SpicyCustomer(*this);
}


// AlchoholicCustomer class
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name, id) , ordered(false),alcDishes(),minAlc(INT8_MAX),curr(INT8_MAX), alcChipInx(INT8_MAX){}


std::vector<int>  AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    if (!ordered ) {
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == ALC && menu[i].getPrice() < minAlc) {
                minAlc = menu[i].getPrice();
                alcChipInx = i;
            }
        }
        ordered = true;
        if (alcChipInx < INT8_MAX )
            return std::vector<int>(alcChipInx);
        else return std::vector<int>();
    }
    else {


        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == ALC && menu[i].getPrice() > minAlc && menu[i].getPrice() <= curr ) {
                curr = menu[i].getPrice();
                alcChipInx = i;
            }
        }
        minAlc = curr;
        curr = INT8_MAX;
       if(alcChipInx < INT8_MAX)
           return std::vector<int>(alcChipInx);
       else return std::vector<int>();


    }


}



std::string AlchoholicCustomer::toString() const {return "";}//TODO: tostring

Customer *AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}
