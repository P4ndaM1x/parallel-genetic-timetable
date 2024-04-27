#pragma once

#include <array>
#include <list>
#include <climits>

#include "Class.hpp"
#include "Timetable.hpp"


class Chromosome {
public:
    using TimeSlot = std::vector<Class::ID>;

    Chromosome(const Timetable& timetable);

    void init(const std::vector<Class>& classes);
    void mutate();
    int calculateFitness();
    int getFitness();
    void printSolution();


private:
    bool isIntervalValid(Class::Time a, Class::Time b);


    std::array<TimeSlot, Timetable::numberOfSlots> timeSlots;
    const Timetable& timetable;
    int fitness { INT_MAX };
};
