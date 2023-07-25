
#include <iostream>
#include "../include/Trainer.h"

using namespace std;

Trainer::Trainer(int t_capacity) :capacity(t_capacity),open(false),salary(0),customersList(vector<Customer*>()),orderList(vector<OrderPair>()){}

int Trainer::getCapacity() const {return capacity;}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Trainer::removeCustomer(int id) {
    for(auto it = customersList.begin(); it != customersList.end(); ++it){ //removes customer from customers list
        if((*it)->getId() == id){
            customersList.erase(it);
            break;
        }
    }

    //changes the id in the orderPairs of the customer to be -1
    for(auto it = orderList.begin(); it != orderList.end(); ++it){ //removes customer's orders from order list
        if((*it).first == id){
            (*it).first = -1;
        }
    }
}

Customer *Trainer::getCustomer(int id) {
    for(auto it = customersList.begin(); it != customersList.end(); ++it){
        if((*it)->getId() == id){
            return *it;
        }
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {return customersList;}

int Trainer::getNumOfCustomers() {
    return customersList.size();
}

std::vector <OrderPair> &Trainer::getOrders() {return orderList;}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids,
                    const std::vector <Workout> &workout_options) {

    if (!getCustomer(customer_id)->hasOrdered){ //check if order is already placed
        //adds order pairs of customers orders to orderList
        for(auto it = workout_ids.begin(); it != workout_ids.end(); ++it){
            pair<int, Workout> orderPair = pair<int,Workout>(customer_id, workout_options[*it]);
            orderList.push_back(orderPair);

            salary = salary + workout_options[*it].getPrice();
        }
        getCustomer(customer_id)->hasOrdered = true;
    }
}

void Trainer::openTrainer() {open = true;}

void Trainer::closeTrainer() {
    open = false;
    //deletes all its customers and removes all its orders
    for(auto it = customersList.begin(); it != customersList.end(); ++it){
        if(*it){
            delete *it;
        }
    }
    customersList.clear();
    orderList.clear();
}

int Trainer::getSalary() {
    return salary;
}

bool Trainer::isOpen() {return this->open;}

void Trainer::setSalary(int amount) {
    salary = salary + amount;
}


Trainer::~Trainer() { //destructor
    //deletes the customers
    for(auto it = customersList.begin(); it != customersList.end(); ++it){
        if(*it){
            delete *it;
        }
    }
    customersList.clear();
}

Trainer::Trainer(const Trainer &other) :capacity(other.capacity),open(other.open),
salary(other.salary),customersList(vector<Customer*>()),orderList(vector<OrderPair>()){ //copy constructor

    //copies all customers of other
    for(auto it = other.customersList.begin(); it != other.customersList.end(); ++it){
        if(dynamic_cast<SweatyCustomer *>(*it)){
            customersList.push_back( new SweatyCustomer((*it)->getName(),(*it)->getId()));
        }else if(dynamic_cast<CheapCustomer *>(*it)){
            customersList.push_back( new CheapCustomer((*it)->getName(),(*it)->getId()));
        }else if(dynamic_cast<HeavyMuscleCustomer *>(*it)){
            customersList.push_back( new HeavyMuscleCustomer((*it)->getName(),(*it)->getId()));
        }else if(dynamic_cast<FullBodyCustomer *>(*it)){
            customersList.push_back( new FullBodyCustomer((*it)->getName(),(*it)->getId()));
        }
    }
    //copies the orders
    for(auto it = other.orderList.begin(); it != other.orderList.end(); ++it){
        orderList.emplace_back(OrderPair((*it).first, (*it).second));
    }
}

Trainer &Trainer::operator=(const Trainer &other) { //copy assignment operator
    if(this == &other){
        return *this;
    }
    open = other.open;
    capacity = other.capacity;
    salary = other.salary; //accumulated

    //deletes source customers
    for(auto it = customersList.begin(); it != customersList.end(); ++it){
        delete *it;
    }
    customersList.clear();

    //copies all customers of other
    for(auto it = other.customersList.begin(); it != other.customersList.end(); ++it) {
        if (dynamic_cast<SweatyCustomer *>(*it)) {
            customersList.push_back(new SweatyCustomer((*it)->getName(), (*it)->getId()));
        } else if (dynamic_cast<CheapCustomer *>(*it)) {
            customersList.push_back(new CheapCustomer((*it)->getName(), (*it)->getId()));
        } else if (dynamic_cast<HeavyMuscleCustomer *>(*it)) {
            customersList.push_back(new HeavyMuscleCustomer((*it)->getName(), (*it)->getId()));
        } else if (dynamic_cast<FullBodyCustomer *>(*it)) {
            customersList.push_back(new FullBodyCustomer((*it)->getName(), (*it)->getId()));
        }

    }
    //clears orderList and copies the orders of other
    orderList.clear();
    for(auto it = other.orderList.begin(); it != other.orderList.end(); ++it){
        orderList.emplace_back(OrderPair((*it).first, (*it).second));
    }
    return *this;
}

Trainer::Trainer(Trainer&& other) : capacity(other.capacity),open(other.open),salary(other.salary),
customersList(other.customersList),orderList(vector<OrderPair>()){ //move constructor

    other.customersList.clear(); //so other won't point to the customers

    //takes others orders
    for(size_t i = 0 ; i < other.orderList.size(); i = i + 1){
        orderList.push_back(other.orderList[i]);
    }
    other.orderList.clear();

}

const Trainer& Trainer::operator=(Trainer &&other) { //move assignment operator
    capacity = other.capacity;
    open = other.open;
    salary = other.salary;

    //takes others customers
    for(auto it = customersList.begin(); it != customersList.end(); ++it){
        if(*it){
            delete *it;
        }
    }
    customersList = other.customersList;
    other.customersList.clear();

    //takes other orders
    orderList.clear();
    for(vector<OrderPair>::iterator it = other.orderList.begin(); it != other.orderList.end(); ++it){
        orderList.push_back(*it);
    }
    other.orderList.clear();

    return *this;
}




