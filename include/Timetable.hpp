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
    Timetable(std::vector<Class>& classes);
    Timetable() = default;

    void addClasses(std::initializer_list<Class> classList);
    ClassContainer getClasses() const { return classes; }
    void printClasses() const;
    void updateClasses(const ClassContainer& newClasses) { classes = newClasses; }
    void print() const;
    std::string serialize() const;

    static Timetable deserialize(const std::string& serializedString);

private:
    ClassContainer classes;
};
