//
// Created by sharon on 06/11/2021.
//

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "../include/Workout.h"
Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type): id(w_id),name(w_name),price(w_price),type(w_type) {}

int Workout::getId() const {return id;}

std::string Workout::getName() const {return name;}

int Workout::getPrice() const {return price;}

WorkoutType Workout::getType() const {return type;}

std::string Workout::getTypeStr() const {
    if(type == 0){
        return "Anaerobic";
    }else if(type == 1){
        return "Mixed";
    }else{
        return "Cardio";
    }
    return std::string();
}

std::vector<int> Workout::sort_indexes(std::vector<Workout> workout) {
    std::vector<int> output(workout.size());
    iota(output.begin(),output.end(),0);
    std::stable_sort(output.begin(), output.end(),
                     [&workout](size_t i1, size_t i2) {return workout[i1].getPrice() < workout[i2].getPrice();});
    return output;
}



//bool Workout::operator<(const Workout &other) {
//    return getPrice() < other.getPrice();
//}
//
//bool Workout::operator!=(const Workout &other) {
//    return getPrice() != other.getPrice();
//}
//
//bool Workout::operator-(const Workout &other) {
//    return getPrice() - other.getPrice();
//}
//
//const Workout &Workout::operator=(const Workout &&workout) {
//    return <#initializer#>;
//}
//
//const Workout & Workout::operator=(const Workout &workout) {
//    if(this == &workout){
//        return *this;
//    }
//    return workout;
//}






