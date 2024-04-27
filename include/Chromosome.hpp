#pragma once

#include <array>
#include <climits>

#include "Class.hpp"
#include "Timetable.hpp"

class Chromosome {
public:
    using TimeSlot = std::vector<Class::ID>;
    using TimeSlotContainer = std::array<TimeSlot, Timetable::numberOfSlots>;

    Chromosome(const Timetable::ClassContainer& classes);

    void init();
    void mutate();
    int calculateFitness();
    int getFitness();
    void printSolution();

private:
    bool isIntervalValid(Class::Time a, Class::Time b);

    TimeSlotContainer timeSlots;
    Timetable::ClassContainer classes;
    int fitness { INT_MAX };
};
