
#include "../include/Action.h"

BaseAction::BaseAction():errorMsg(""),status(COMPLETED){}

ActionStatus BaseAction::getStatus() const {return status;}


void BaseAction::complete() {status = COMPLETED;}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    std::cout<< "Error: " << errorMsg << std::endl;
    BaseAction::errorMsg = "Error: "+errorMsg;
}


std::string BaseAction::getErrorMsg() const {return errorMsg;}


void BaseAction::setStatus(ActionStatus actionStatus) {
    status = actionStatus;
}
void BaseAction::setErrorMsg(std::string s) {
    errorMsg = s;
}
std::string BaseAction::getErrStr() const{
    if(getStatus() == ERROR){
        return getErrorMsg();
    }else{
        return "Completed";
    }
}


BaseAction::~BaseAction()= default;


OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) :trainerId(id), customers(customersList),customersStr(""){

}

void OpenTrainer::act(Studio &studio) {
    if(studio.getTrainer(trainerId) == nullptr || studio.getTrainer(trainerId)->isOpen() | (studio.getTrainer(trainerId)->getCapacity() < studio.getTrainer(trainerId)->getNumOfCustomers())){
        error("Workout session does not exist or is already open.");
        return;
    }

    //opens the trainer and adds the customers to its customers list
    Trainer* trainer = studio.getTrainer(trainerId);
    trainer->openTrainer();
    for(std::vector<Customer*>::iterator it = customers.begin(); it != customers.end(); ++it){
        trainer->addCustomer(*it);
        customersStr.append((*it)->toString() + " ");
    }

    complete();
}

std::string OpenTrainer::toString() const { //returns string for the action log

    //return customersStr + getErrStr(); //we updated customerStr during buildAction
    return "open " + std::to_string(trainerId) +" "+ customersStr + getErrStr() ;
}

void OpenTrainer::setCustomerStr(std::string s) {
    customersStr = s;
}

std::string OpenTrainer::getCustomerStr() {
    return customersStr;
}

OpenTrainer::~OpenTrainer() = default;


Order::Order(int id) :trainerId(id){}

void Order::act(Studio &studio) {
    if(studio.getTrainer(trainerId) == nullptr || !studio.getTrainer(trainerId)->isOpen()){
        error("Trainer does not exist or is not open.");
        return;
    }

    Trainer* trainer = studio.getTrainer(trainerId);
    std::vector<Customer*> customers = trainer->getCustomers();
    std::vector<Workout>& workoutOptions = studio.getWorkoutOptions();
    for(std::vector<Customer*>::iterator it = customers.begin(); it != customers.end(); ++it){ //orders foe each customer
        std::vector<int> orderedWorkoutsIds = (*it)->order(studio.getWorkoutOptions());
        trainer->order((*it)->getId(),orderedWorkoutsIds,workoutOptions);
        for(int id: orderedWorkoutsIds){
            std::cout << (*it)->getName() << " Is Doing " << workoutOptions[id].getName() <<std::endl;
        }
    }
    complete();
}

std::string Order::toString() const {
    return "order " + std::to_string(trainerId)+ " "+ getErrStr();
}
Order::~Order()= default;;




MoveCustomer::MoveCustomer(int src, int dst, int customerId) :srcTrainer(src), dstTrainer(dst),id(customerId){}

void MoveCustomer::act(Studio &studio) {
    if(studio.getTrainer(srcTrainer) == nullptr || studio.getTrainer(dstTrainer) == nullptr){
        error("Cannot move customer");
        return;
    }
    Trainer* sTrainer = studio.getTrainer(srcTrainer);
    Trainer* dTrainer = studio.getTrainer(dstTrainer);
    Customer* customer = sTrainer->getCustomer(id);
    if((customer == nullptr) | (!sTrainer->isOpen()) | (!dTrainer->isOpen()) | (dTrainer->getCapacity() == dTrainer->getNumOfCustomers())){
        error("Cannot move customer");
        return;
    }

    //moves the orders to the destination trainer
    std::vector<OrderPair>& sOrderPair = sTrainer->getOrders();
    std::vector<OrderPair>& dOrderPair = dTrainer->getOrders();
    int price =0;
    for(auto & it : sOrderPair){ //adds the orders of the customer to the dest trainer
        if(it.first == id){
            dOrderPair.push_back(it);
            price = price + (it).second.getPrice();
        }
    }
    //updates salaries
    sTrainer->setSalary(-price);
    dTrainer->setSalary(price);

    //moves the customer
    dTrainer->addCustomer(customer);
    sTrainer->removeCustomer(id); //also erases its orders

    //if the trainer has no more customers, closes it
    if(sTrainer->getCustomers().empty()){
        sTrainer->closeTrainer();
    }
    complete();
}

std::string MoveCustomer::toString() const {

    return "move " + std::to_string(srcTrainer)+ " "+ std::to_string(dstTrainer)+" "+ std::to_string(id)+ " "+ getErrStr();
}
MoveCustomer::~MoveCustomer()= default;;






Close::Close(int id): trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if(trainer == nullptr || !studio.getTrainer(trainerId)->isOpen()){
        error("Trainer does not exist or is not open");
        return;
    }
    trainer->closeTrainer();
    std::cout<< "Trainer "<< trainerId << " closed. Salary "<< trainer->getSalary()<<"NIS" << std::endl;
    complete();
}

std::string Close::toString() const {
    return "close " + std::to_string(trainerId)+ " "+ getErrStr();
}
Close::~Close() = default;;


CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    studio.close();
    complete();
}

std::string CloseAll::toString() const {
    return "closeall "+ getErrStr();
}
CloseAll::~CloseAll() = default;



PrintWorkoutOptions::PrintWorkoutOptions() = default;

void PrintWorkoutOptions::act(Studio &studio) {
    studio.printWorkoutOptions();
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options "+ getErrStr();
}
PrintWorkoutOptions::~PrintWorkoutOptions() = default;;



PrintTrainerStatus::PrintTrainerStatus(int id) :trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    std::string status;
    //prints trainer status
    if(trainer->isOpen()){
       status = "open";
    }else{
        status = "closed";
    }
    std::cout <<"Trainer "<<trainerId<< " status: "<< status <<std::endl;
    if(trainer->isOpen()){
        //prints customers of trainer
        std::cout<<"Customers:"<<std::endl;
        std::vector<Customer*> customers = trainer->getCustomers();
        std::vector<OrderPair> orders = trainer->getOrders();
        for(auto it = customers.begin(); it != customers.end(); ++it){
            std::cout << (*it)->getId() <<" "<< (*it)->getName()<<std::endl;
        }
        //prints orders of trainers
        std::cout<<"Orders:"<<std::endl;
        for(auto it = orders.begin(); it != orders.end(); ++it){
            if((*it).first != -1){ //ignores customers with id -1
                std::cout <<(*it).second.getName()<< " " << (*it).second.getPrice()<< "NIS "<< (*it).first <<std::endl;
            }
        }
        //prints the salary
        std::cout <<"Current Trainer's Salary: "<<trainer->getSalary() <<"NIS"<<std::endl;
    }
    complete();
}

std::string PrintTrainerStatus::toString() const {
    return "status " + std::to_string(trainerId)+ " "+ getErrStr();
}
PrintTrainerStatus::~PrintTrainerStatus() = default;;



PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction*> al = studio.getActionsLog();

    for(std::vector<BaseAction*>::iterator it = al.begin(); it != al.end(); ++it){
        std::cout << (*it)->toString() <<std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log "+ getErrStr();
}
PrintActionsLog::~PrintActionsLog() = default;



BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    if (backup == nullptr){
        backup = new Studio(studio); //calls copy constructor of studio
    }else{
        *backup = studio; //calls copy assignment operator of studio
    }
    complete();
}

std::string BackupStudio::toString() const {
    return "backup "+ getErrStr();
}

BackupStudio::~BackupStudio()= default;



RestoreStudio::RestoreStudio() {
}

void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr){
        error("No backup available");
        return;
    }
    studio = *backup; //calls copy assignment operator of studio
    complete();
}

std::string RestoreStudio::toString() const {
    return "restore "+ getErrStr();
}
RestoreStudio::~RestoreStudio() = default;