#pragma once

#include "Class.hpp"

#include <vector>

class Class;

// stores and manages input data
class Timetable {
public:
    static constexpr unsigned numberOfDays = 5;
    static constexpr unsigned slotsPerDay = 8;
    static constexpr std::size_t numberOfSlots = numberOfDays * slotsPerDay;
    using ClassContainer = std::vector<Class>;

    void addClasses(std::initializer_list<Class> classList);
    ClassContainer getClasses() const { return classes; }

private:
    ClassContainer classes;
};
