/**
 * @file Timetable.cpp
 * @brief Implementation file of the Timetable class.
 */
#include "Timetable.hpp"

#include "Chromosome.hpp"

void Timetable::addClasses(std::initializer_list<Class> classList)
{
    for (const auto& c : classList) {
        classes.push_back(c);
    }
}

void Timetable::printClasses() const
{
    for (const auto& c : classes)
        std::cout << c << std::endl;
}

void Timetable::print() const { Chromosome{classes, false}.printSolution(); }

Timetable::Timetable(const ClassContainer& classes)
{
    for (const auto& c : classes) {
        this->classes.push_back(c);
    }
}

std::string Timetable::serializeClasses(const ClassContainer& classes)
{
    std::stringstream ss;
    for (const auto& c : classes) {
        ss << c.serialize() << ";";
    }
    return ss.str();
}

Timetable::ClassContainer Timetable::deserializeClasses(const std::string& serializedClasses)
{
    std::stringstream ss{serializedClasses};
    std::string line;

    Timetable::ClassContainer classes;

    while (std::getline(ss, line, ';')) {
        classes.push_back(Class::deserialize(line));
    }

    return classes;
}