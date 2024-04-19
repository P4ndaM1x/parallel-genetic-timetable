#pragma once

#include "Class.hpp"

#include <vector>

// stores and manages input data
class Timetable {
public:
    static constexpr unsigned numberOfDays = 5;
    static constexpr unsigned slotsPerDay = 8;
    static constexpr std::size_t numberOfSlots = numberOfDays * slotsPerDay;

    using data_type = std::vector<Class>;

    Timetable(std::initializer_list<Class> classList);

    void addClasses(std::initializer_list<Class> classList);
    data_type getClasses() const { return classes; }
    void setSolution(const data_type& classes) { this->classes = classes; }

    void printClasses() const;
    void printSolution() const;

private:
    data_type classes;
};
