#pragma once

#include "Class.hpp"

#include <vector>

// stores and manages input data
class Timetable {
public:
    static constexpr unsigned numberOfDays = 5;
    static constexpr unsigned slotsPerDay = 8;
    static constexpr std::size_t numberOfSlots = numberOfDays * slotsPerDay;

    void addClasses(std::initializer_list<Class> classList);
    std::vector<Class> getClasses() const { return classes; }
    void setSolution(const std::vector<Class>& classes) { this->classes = classes; }

    void printClasses() const;
    void printSolution() const;

private:
    std::vector<Class> classes;
};
