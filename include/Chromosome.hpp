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

    Chromosome(const Timetable::ClassContainer& classes, const bool randomizeStartTimes = true);

    void init();
    void mutate();
    uint32_t calculateError();
    uint32_t getError() const;
    void printSolution() const;
    Class getClass(const Class::ID) const;
    Timetable::ClassContainer getClasses() const;

private:
    bool isIntervalValid(Class::Time a, Class::Time b);
    void updateTimeSlotContainer();

    TimeSlotContainer timeSlots;
    Timetable::ClassContainer classes;
    uint32_t error{std::numeric_limits<uint32_t>::max()};
};
