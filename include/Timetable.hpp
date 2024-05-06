/**
 * @file Timetable.hpp
 * @brief Header file for the Timetable class.
 * @see Timetable.cpp
 */
#pragma once

#include "Class.hpp"

#include <string>
#include <vector>

class Class;

class Timetable {
public:
    static constexpr unsigned numberOfDays = 5;
    static constexpr unsigned slotsPerDay = 8;
    static constexpr std::size_t numberOfSlots = numberOfDays * slotsPerDay;

    using ClassContainer = std::vector<Class>;
    Timetable(const ClassContainer& classes);
    Timetable() = default;

    void addClasses(std::initializer_list<Class> classList);
    ClassContainer getClasses() const { return classes; }
    void printClasses() const;
    void updateClasses(const ClassContainer& newClasses) { classes = newClasses; }
    void print() const;

    static std::string serializeClasses(const ClassContainer& classes);
    static ClassContainer deserializeClasses(const std::string& serializedClasses);

private:
    ClassContainer classes;
};
