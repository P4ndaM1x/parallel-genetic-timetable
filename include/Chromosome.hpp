#pragma once

#include "Class.hpp"
#include "Timetable.hpp"

#include <array>
#include <cstdint>
#include <limits>

class Chromosome {
public:
    using TimeSlot = std::vector<Class::ID>;
    using TimeSlotContainer = std::array<TimeSlot, Timetable::numberOfSlots>;

    Chromosome(const Timetable::ClassContainer& classes);

    void init();
    void mutate();
    uint32_t calculateFitness();
    uint32_t getFitness();
    void printSolution();

private:
    bool isIntervalValid(Class::Time a, Class::Time b);

    TimeSlotContainer timeSlots;
    Timetable::ClassContainer classes;
    uint32_t fitness { std::numeric_limits<uint32_t>::max() };
};
