#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include "Customer.h"

class BaseAction;

class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();

    BaseAction* buildAction(std::string command);
    BaseAction* buildActionOpen(std::string command);
    BaseAction* buildActionOrder(std::string command);
    BaseAction* buildActionMove(std::string command);
    BaseAction* buildActionClose(std::string command);
    BaseAction* buildActionCloseAll();
    BaseAction* buildActionWorkoutOptions();
    BaseAction* buildActionPrintStatus(std::string command);
    BaseAction* buildActionPrintLog();
    BaseAction* buildActionBackup();
    BaseAction* buildActionRestore();

    void close();
    void printWorkoutOptions();

    virtual ~Studio();//destructor
    Studio(const Studio& other); //copy constructor
    Studio& operator=(const Studio& other); //copy assignment operator
    Studio(Studio&& other); //move constructor
    const Studio& operator=(Studio&& other); //move operator =

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int numOfCustomers;
};

extern Studio* backup;

#endif