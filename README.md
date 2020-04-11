# Restaurant
C++ program that simulates a restaurant management system. The program will open the restaurant, 
assign customers to tables, make orders, provide bills to the tables, and other request.

The program will get a config file as an input, which includes all required information about the
restaurant opening - number of tables, number of available seats in each table, and details
about the dishes in the menu. 

There are 4 types of customers in this restaurant, each customer type has its own way of
ordering from the menu (an ordering strategy). An order may be taken from a table more than
once, in such cases some customers may order a different dish.

Each table in the restaurant has a limited amount of seats available (this info is provided in the
config file). The restaurant can’t connect tables together, nor accommodates more customers
than the number of seats available in a table. In this restaurant, it’s impossible to add new
customers to an open table, but it’s possible to move customers from one table to another.
A bill of a table is the total price of all dishes ordered for that table. 

# The Program flow

The program receives the path of the config file as the first command line argument. Once the
program starts, it opens the restaurant by calling the start() function, followed by printing
“Restaurant is now open!” to the screen.

Then the program waits for the user to enter an action to execute. After each executed action,
the program waits for the next action in a loop. The program ends when the user enters the
action "closeall"

# Classes
**Restaurant** – This class holds a list of tables, menu, and other information that is relevant to
the restaurant.

**Table** – This class represents a table in the restaurant. Each table has a finite number of
available seats (provided in the config file). It also holds a status flag that indicates whether the
table is open, a list of orders done in this table, and a list of customers.

**Dish** – This class represents a dish in the menu. It has an id, name, price, and a type.

**Customer** – This is an abstract class for the different customers classes. There are several
types of customers, and each of them has a different ordering strategy. Each customer that
arrives to the restaurant will get a number (id) that will serve as an identifier as long as he is
seating in the restaurant. This number will be a serial number of all customers that arrived so
far, starting from 0 (the first customer will get 0, second customer will get 1 , etc.).
The class has a pure virtual method order(menu) which receives the menu, and returns a
vector of dishes IDs that were ordered by the customers.

**BaseAction** – This is an abstract class for the different action classes. The class contains a
pure virtual method act(Restaurant& rest) which receives a reference to the restaurant as a
parameter and performs an action on it; A pure virtual method “toString()” which returns a 
string representation of the action; A flag which stores the current status of the action:
“Pending” for actions that weren't performed yet, “Completed” for successfully completed
actions, and “Error” for actions which couldn't be completed.

# Built With
- [OOP](https://en.wikipedia.org/wiki/Object-oriented_programming)
- [Rule Of Five](https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)#Rule_of_Five)

### Tech
* [C++](https://www.cplusplus.com/)
* [CLion](https://www.jetbrains.com/clion/)
* [Valgrind](https://valgrind.org/)
* [Github](https://github.com/)

### Authors
* Amit Levizky
* Evgeny Kaidrikov
