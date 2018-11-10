//
// Created by Amit Levizky on 11/3/18.
//

#include "Customer.h"

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

//End of VegetarianCustomer class


//CheapCustomer class
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id), ordered(false), cheapest(1000){
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
    }

    if (menu[cheapest].getId() < 1000)
        return std::vector<int>(cheapest);
    return std::vector<int>();
}

std::string CheapCustomer::toString() const {return "";}//TODO: toString

//End of CheapCustomer class

//SpicyCustomer class
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id){
}