#pragma once

#include <array>
#include <list>
#include <climits>

#include "Class.hpp"
#include "Timetable.hpp"


class Chromosome {
public:
    using TimeSlot = std::vector<Class::ID>;

    Chromosome(const std::vector<Class>& classes);

    std::vector<Class> getClasses() const { return classes; }

    void randomize();
    void mutate();
    int calculateFitness();
    int getFitness();
    void printSolution();

private:
    std::array<TimeSlot, Timetable::numberOfSlots> timeSlots;
    std::vector<Class> classes;
    int fitness { INT_MAX } ;
};
