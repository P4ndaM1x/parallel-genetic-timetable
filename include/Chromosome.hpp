#pragma once

#include "Class.hpp"
#include "Timetable.hpp"

#include <array>
#include <list>

class Chromosome {
public:
    using underlying_type = std::array<std::list<Class::id_type>, Timetable::numberOfSlots>;

    Chromosome(const Timetable::data_type& classes);

    Timetable::data_type getClasses() const { return classes; }

    void randomize();
    void mutate();
    void fitness();

private:
    underlying_type timeSlots;
    Timetable::data_type classes;
};
