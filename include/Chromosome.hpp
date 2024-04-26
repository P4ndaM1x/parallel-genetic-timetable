#pragma once

#include "Class.hpp"
#include "Timetable.hpp"

#include <array>
#include <list>

class Chromosome {
public:
    using TimeSlot = std::vector<Class::ID>;

    Chromosome(const std::vector<Class>& classes);

    std::vector<Class> getClasses() const { return classes; }

    void randomize();
    void mutate();
    void fitness();

private:
    std::array<TimeSlot, Timetable::numberOfSlots> timeSlots;
    std::vector<Class> classes;
};
