#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "../include/Studio.h"

using namespace std;

    Studio::Studio():open(false), trainers(vector<Trainer*>()),
    workout_options(vector<Workout>()),
    actionsLog(vector<BaseAction*>()),
    numOfCustomers(0){}

    Studio::Studio(const std::string &configFilePath) : Studio(){
        ifstream file(configFilePath);
        string line;
        int i = 0;
        while(getline(file, line)){
            if((line.size() == 0) || (line.at(0) == '#') | (line.at(0) == '\0')){
                continue;
            }
            if(i == 0){ //creates the vector of trainers and sets its size
                int numOfTrainers = line.at(0) - '0';
                trainers = vector<Trainer*>(numOfTrainers);
            }else if(i == 1){ //creates the trainer with their capacities
                istringstream ss(line);
                string capacityStr;
                int j = 0;
                while(getline(ss,capacityStr,',')){
                    if(j < getNumOfTrainers()){
                        capacityStr.erase(std::remove(capacityStr.begin(),capacityStr.end(),' '), capacityStr.end());
                        int capacity;
                        istringstream (capacityStr) >> capacity;
                        if(capacity){
                            Trainer* trainer = new Trainer(capacity);
                            trainers[j] = trainer;
                        }
                    }
                    j = j + 1;
                }
            } else{ //creates workouts and adds them to workout_options
                istringstream ss(line);
                string current;
                int j = 0;
                string workoutName;
                string workoutType;
                string workoutPrice;
                int type;
                while(getline(ss,current,',')){
                    if(j != 0){
                        current.erase(std::remove(current.begin(),current.end(),' '),current.end());
                    }
                    if(j == 0){
                        workoutName = current;
                    }else if( j == 1){
                        workoutType = current;
                    }else{
                        workoutPrice = current;
                    }
                    j = j + 1;
                }
                if(workoutType == "Anaerobic"){
                    type = 0;
                }else if(workoutType == "Cardio"){
                    type = 2;
                }else{
                    type = 1;
                }
                workout_options.emplace_back(workout_options.size(),
                                                  workoutName,
                                                  std::stoi(workoutPrice),
                                                  static_cast<WorkoutType>(type));
            }
            i++;
        }
}


void Studio::start() {
    open = true;
    cout << "Studio is now open!" << endl;
    std::string command;
    std::getline(std::cin, command);

    while(command != "closeall"){
        BaseAction* action = buildAction(command);
        if(dynamic_cast<BackupStudio*>(action)){ //so the backup will have the backup action in action log
            actionsLog.push_back(action);
            action->act(*this);
        }else{
            action->act(*this);
            actionsLog.push_back(action);
        }
        std::getline(std::cin, command);
    }
    BaseAction* action = buildAction(command);
    action->act(*this);
    actionsLog.push_back(action);
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    if((tid < 0) | (tid >= getNumOfTrainers())){
        return nullptr;
    }
    return trainers[tid];
}


const std::vector<BaseAction*>& Studio::getActionsLog() const {
    return actionsLog;
}// Return a reference to the history of actions

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}


BaseAction *Studio::buildAction(std::string command) {
    istringstream ss(command);
    size_t space_pos = command.find(" ");
    string action;
    if(space_pos != string::npos){
        action = command.substr(0,space_pos);
    }else{
        action = command;
    }

    if(action == "open"){
        return buildActionOpen(command);
    }else if(action == "order"){
        return buildActionOrder(command);
    }else if(action == "move"){
        return buildActionMove(command);
    }else if(action == "close"){
        return buildActionClose(command);
    }else if(action == "closeall"){
        return buildActionCloseAll();
    }else if(action == "workout_options"){
        return buildActionWorkoutOptions();
    }else if(action == "status"){
        return buildActionPrintStatus(command);
    }else if(action == "log"){
        return buildActionPrintLog();
    }else if(action == "backup"){
        return buildActionBackup();
    }else{//restore
        return buildActionRestore();
    }
}
BaseAction* Studio::buildActionOpen(string command) {
    istringstream ss(command);
    string current;
    int i = 0;
    int trainerId;
    string name;
    string type;
    bool hasTrainer = false;
    int numOfCustomersAdded = 0;
    vector<Customer*> customers;
        while(getline(ss,current,' ')){
            current.erase(std::remove(current.begin(),current.end(),' '),current.end());
            if((current != "Error:") & (current != "Completed")){
                if(i == 1){ //sets the trainer id
                    trainerId = stoi(current);
                    if(!(getTrainer(trainerId) == nullptr || getTrainer(trainerId)->isOpen())){
                        hasTrainer = true;
                    }
                }else if(i > 1 and hasTrainer){ //sets the customers
                    istringstream ss1(current);
                    string detail;
                    int j = 0;
                    while(getline(ss1,detail,',')){
                        if(j == 0){
                            name = detail;
                        }else{
                            type = detail;
                        }
                        j++;
                    }
                    if(getTrainer(trainerId)->getCapacity() > numOfCustomersAdded){
                        Customer* customer;
                        if (type == "swt") {
                            customer = new SweatyCustomer(name, numOfCustomers);
                        }else if(type == "chp"){
                            customer = new CheapCustomer(name, numOfCustomers);
                        }else if(type == "mcl"){
                            customer = new HeavyMuscleCustomer(name, numOfCustomers);
                        }else if (type == "fbd"){
                            customer = new FullBodyCustomer(name, numOfCustomers);
                        }
                        customers.push_back(customer);
                        numOfCustomersAdded++;
                        numOfCustomers++;
                    }
                }

            }
            i++;
    }
    BaseAction* openAction = new OpenTrainer(trainerId,customers);
    return openAction;
}

BaseAction *Studio::buildActionOrder(string command) {
    istringstream ss(command);
    string current;
    int i = 0;
    int trainerId;
    while(getline(ss,current,' ')){ //sets the trainer id
        current.erase(std::remove(current.begin(),current.end(),' '),current.end());
        if((i == 1) & (current!= "Error:") & (current != "Completed")){
            trainerId = stoi(current);
        }
        i++;
    }
    BaseAction* action = new Order(trainerId);
    return action;
}

BaseAction *Studio::buildActionMove(string command){
    istringstream ss(command);
    string current;
    int i = 0;
    int srcTrainerId;
    int dstTrainerId;
    int customerId;
    while(getline(ss,current,' ')){
        current.erase(std::remove(current.begin(),current.end(),' '),current.end());
        if((current != "Error:") & (current != "Completed")){
            if(i == 1){
                srcTrainerId = stoi(current);
            }else if(i == 2){
                dstTrainerId = stoi(current);
            }else if(i == 3){
                customerId = stoi(current);
            }
        }
        i++;
    }
    BaseAction* action = new MoveCustomer(srcTrainerId,dstTrainerId,customerId);
    return action;
}

BaseAction *Studio::buildActionClose(string command){
    istringstream ss(command);
    string current;
    int i = 0;
    int trainerId;
    while(getline(ss,current,' ')){ //sets the trainer id
        current.erase(std::remove(current.begin(),current.end(),' '),current.end());
        if((i == 1) & (current != "Error:") & (current != "Completed")){
            trainerId = stoi(current);
        }
        i++;
    }
    BaseAction* action = new Close(trainerId);
    return action;
}

BaseAction *Studio::buildActionCloseAll(){
    BaseAction* action = new CloseAll();
    return action;
}

BaseAction *Studio::buildActionWorkoutOptions() {
    BaseAction* action = new PrintWorkoutOptions();
    return action;
}

BaseAction *Studio::buildActionPrintStatus(string command) {
    istringstream ss(command);
    string current;
    int i = 0;
    int trainerId;
    while(getline(ss,current,' ')){ //sets the trainer id
        current.erase(std::remove(current.begin(),current.end(),' '),current.end());
        if((i == 1) & (current != "Error:") & (current != "Completed")){
            trainerId = stoi(current);
        }
        i++;
    }
    BaseAction* action = new PrintTrainerStatus(trainerId);
    return action;
}

BaseAction* Studio::buildActionPrintLog() {
    BaseAction* action = new PrintActionsLog();
    return action;
}

BaseAction *Studio::buildActionBackup() {
    BaseAction* action = new BackupStudio();
    return action;
}

BaseAction *Studio::buildActionRestore() {
    BaseAction* action = new RestoreStudio();
    return action;
}


//closes studio and all trainers
void Studio::close() {
    open = false;
    for(size_t i = 0; i != trainers.size(); i = i+1){
        if(getTrainer(i)->isOpen()){
            Close close = Close(i);
            close.act(*this);
        }
    }
}

void Studio::printWorkoutOptions() {
    for(auto it = workout_options.begin(); it != workout_options.end(); ++it){
        cout << (*it).getName() << ", " << (*it).getTypeStr() << ", " << (*it).getPrice() << endl;
    }
}


Studio::~Studio() {
    for(auto it = trainers.begin(); it != trainers.end(); ++it){
        if(*it){
            delete *it;
        }
    }
    for(auto it = actionsLog.begin(); it != actionsLog.end(); ++it){
        if(*it){
            delete *it;
        }
    }
}

Studio::Studio(const Studio& other) : open(other.open),trainers(vector<Trainer*>()),
workout_options(vector<Workout>()),actionsLog(vector<BaseAction*>()) ,numOfCustomers(other.numOfCustomers){ //copy constructor
    //copy trainers
    vector<Trainer*> otherStudioTrainers = other.trainers;
    for(size_t t = 0; t < otherStudioTrainers.size(); t = t + 1){
        trainers.push_back(new Trainer(*otherStudioTrainers[t]));
    }
    //copy workouts
    for(auto it = other.workout_options.begin(); it!=other.workout_options.end(); ++it){
        workout_options.emplace_back(it->getId(),it->getName(),it->getPrice(),it->getType());
    }
    //copy actions
    for(auto it = other.actionsLog.begin(); it!=other.actionsLog.end(); ++it){
        string name = (*it)->toString();
        BaseAction* action = buildAction(name);
        //sets the status of the action
        if((*it)->getStatus() == COMPLETED){
            action->setStatus(COMPLETED);
        }else{
            action->setStatus(ERROR);
            action->setErrorMsg((*it)->getErrStr());
        }

        if (dynamic_cast<OpenTrainer *>(*it)) { //copies the customerStr for the log
            dynamic_cast<OpenTrainer *>(action)->setCustomerStr(dynamic_cast<OpenTrainer *>(*it)->getCustomerStr());
        }
        actionsLog.push_back(action);
    }
}

Studio &Studio::operator=(const Studio& other) { //copy assignment operator
    if(this != &other){
        open = other.open;
        numOfCustomers = other.numOfCustomers;

        //delete source trainers
        for(auto it = trainers.begin(); it != trainers.end(); ++it){
            if(*it){
                delete *it;
            }
        }
        trainers.clear();

        //copy trainers
        vector<Trainer*> otherStudioTrainers = other.trainers;
        for(size_t t = 0; t < otherStudioTrainers.size(); t = t + 1){
            trainers.push_back(new Trainer(*otherStudioTrainers[t]));
        }

        workout_options.clear();
        //copy workouts
        for(auto it = other.workout_options.begin(); it!=other.workout_options.end(); ++it){
            workout_options.push_back(Workout(it->getId(),it->getName(),it->getPrice(),it->getType()));
        }
        for(auto it = actionsLog.begin(); it != actionsLog.end(); ++it){
            if(*it){
                delete *it;
            }
        }
        actionsLog.clear();
        //copy actions
        for(auto it = other.actionsLog.begin(); it!=other.actionsLog.end(); ++it){
            string name = (*it)->toString();
            BaseAction* action = buildAction(name);
            if((*it)->getStatus() == COMPLETED){
                action->setStatus(COMPLETED);
            }else{
                action->setStatus(ERROR);
                action->setErrorMsg((*it)->getErrStr());
            }
            if (dynamic_cast<OpenTrainer *>(*it)) { //copies the customerStr for the log
                dynamic_cast<OpenTrainer *>(action)->setCustomerStr(dynamic_cast<OpenTrainer *>(*it)->getCustomerStr());
            }
            actionsLog.push_back(action);
        }
    }
    return *this;
}

Studio::Studio(Studio &&other) : open(other.open),trainers(other.trainers),
 workout_options(vector<Workout>()),actionsLog(other.actionsLog),numOfCustomers(other.numOfCustomers){ //move constructor

    other.trainers.clear();

    for(vector<Workout>::iterator it = other.workout_options.begin(); it != other.workout_options.end(); ++it){
        workout_options.push_back(*it);
    }
    other.workout_options.clear();

    other.actionsLog.clear();
}

const Studio &Studio::operator=(Studio &&other) { //move assignment operator
    open = other.open;
    numOfCustomers = other.numOfCustomers;

    //delete source trainers
    for(auto it = trainers.begin(); it != trainers.end(); ++it){
        if(*it){
            delete *it;
        }
    }
    trainers = other.trainers;
    other.trainers.clear();

    //move workouts
    workout_options.clear();
    for(vector<Workout>::iterator it = other.workout_options.begin(); it != other.workout_options.end(); ++it){
        workout_options.push_back(*it);
    }
    other.workout_options.clear();

    //move action logs
    for(auto it = actionsLog.begin(); it != actionsLog.end(); ++it){
        if(*it){
            delete *it;
        }
    }
    actionsLog = other.actionsLog;
    other.actionsLog.clear();
    return *this;
}










