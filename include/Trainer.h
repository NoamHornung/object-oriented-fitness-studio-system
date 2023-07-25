#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;
//std::pair<int,Workout>& std::pair<int,Workout>::operator=(const std::pair<int, Workout>&);

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    int getNumOfCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    void setSalary(int amount);

    virtual ~Trainer(); //destructor
    Trainer(const Trainer &other); //copy constructor
    Trainer& operator=(const Trainer &other);//copy assignment operator
    Trainer(Trainer&& other); //move constructor
    const Trainer& operator=(Trainer&& other); //move assignment operator
private:
    int capacity;
    bool open;
    int salary; //accumulated
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    //the pairs are allocated on heap
};


#endif