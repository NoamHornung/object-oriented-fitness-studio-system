
#include "../include/Customer.h"
using namespace std;


Customer::Customer(std::string c_name, int c_id) :hasOrdered(false),name(c_name),id(c_id){}

std::string Customer::getName() const {return name;}

int Customer::getId() const {return id;}

Customer::~Customer() = default;


SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name,id){}

std::vector<int> SweatyCustomer::order(const std::vector <Workout> &workout_options) {
    //orders all the cardio workouts
    vector<int> orderedWorkouts;
    for(::size_t i = 0; i < workout_options.size(); ++i){
        if(workout_options[i].getType() == CARDIO){
            orderedWorkouts.push_back(i);
        }
    }
    return orderedWorkouts;
}

std::string SweatyCustomer::toString() const {
    return getName() + ",swt";
}
SweatyCustomer::~SweatyCustomer(){}


CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id) {}

std::vector<int> CheapCustomer::order(const std::vector <Workout> &workout_options) {
    //finds the workout with the cheapest price and adds its index to output array
    vector<int> orderedWorkouts;
    int price = workout_options[0].getPrice();
    int index = 0;
    for(::size_t i = 1; i < workout_options.size(); ++i){
        Workout workout = workout_options[i];
        if(workout.getPrice() < price){
            price = workout.getPrice();
            index = i;
        }
    }
    orderedWorkouts.push_back(index);
    return orderedWorkouts;
}

std::string CheapCustomer::toString() const {
    return  getName() + ",chp";
}
CheapCustomer::~CheapCustomer(){}


HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name,id){}

std::vector<int> HeavyMuscleCustomer::order(const std::vector <Workout> &workout_options) {
    //finds all anaerobic workouts, sorts them and add them from most expensive to cheapest
    vector<Workout> orderedWorkouts;
    vector<int> output;
    for(::size_t i = 0; i < workout_options.size(); ++i){
        if(workout_options[i].getType() == ANAEROBIC){
            orderedWorkouts.push_back(workout_options[i]);
        }
    }

    std::vector<int> sorted  =  sort_indexes(orderedWorkouts);
    for(int i = sorted.size()-1; i >= 0; --i){
        output.push_back(orderedWorkouts[sorted[i]].getId());
    }

    return output;
}

std::string HeavyMuscleCustomer::toString() const {
    return getName() + ",mcl";
}

HeavyMuscleCustomer::~HeavyMuscleCustomer(){}




FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name,id){}

std::vector<int> FullBodyCustomer::order(const std::vector <Workout> &workout_options) {
    vector<int> orderedWorkouts;
    int cardioPrice = -1;
    int cardioIndex = -1;
    int mixPrice = -1;
    int mixIndex = -1;
    int anaerobicPrice = -1;
    int anaerobicIndex = -1;
    for(::size_t i = 0; i < workout_options.size(); ++i){
        Workout workout = workout_options[i];
        int type = workout.getType();
        if(type == 2){ //Cardio workout
            if (cardioPrice == -1 || workout.getPrice() < cardioPrice){
                cardioPrice = workout.getPrice();
                cardioIndex = i;
            }
        }else if(type == 1){ //mixed workout
            if (workout.getPrice() > mixPrice){
                mixPrice = workout.getPrice();
                mixIndex = i;
            }
        }else{ //anaerobic workout
            if(anaerobicPrice == -1 || workout.getPrice() < anaerobicPrice){
                anaerobicPrice = workout.getPrice();
                anaerobicIndex = i;
            }
        }
    }
    if((cardioIndex ==-1) | (mixIndex == -1) | (anaerobicIndex == -1)){
        orderedWorkouts = {};
    }else{
        orderedWorkouts = {cardioIndex, mixIndex, anaerobicIndex};
    }

    return orderedWorkouts;
}
std::string FullBodyCustomer::toString() const {
    return getName() + ",fbd";
}
FullBodyCustomer::~FullBodyCustomer(){}

//sorts the workouts according to price
std::vector<int> Customer::sort_indexes(std::vector<Workout> workout) {
    std::vector<int> output(workout.size());
    iota(output.begin(),output.end(),0);
    std::stable_sort(output.begin(), output.end(),
                     [&workout](size_t i1, size_t i2) {return workout[i1].getPrice() < workout[i2].getPrice();});
    return output;
}


