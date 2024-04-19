#include "Timetable.hpp"

#include <iostream>

Timetable::Timetable(std::initializer_list<Class> classList)
    : classes { classList }
{
}

void Timetable::printClasses() const
{
}

void Timetable::printSolution() const
{
}

void Timetable::addClasses(std::initializer_list<Class> classList)
{
    for (const auto& c : classList) {
        classes.push_back(c);
    }
}
