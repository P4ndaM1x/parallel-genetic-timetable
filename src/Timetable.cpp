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

void Timetable::print() const
{
    Chromosome { classes, false }.printSolution();
}
