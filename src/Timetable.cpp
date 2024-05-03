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

Timetable::Timetable(std::vector<Class>& classes)
{
    for (const auto& c : classes) {
        this->classes.push_back(c);
    }
}

std::string Timetable::serialize() const
{
    std::stringstream ss;
    for (const auto& clazz : classes) {
        ss << clazz.serialize() << "\n";
    }
    return ss.str();
}

Timetable Timetable::deserialize(const std::string& serializedString)
{
    std::stringstream ss(serializedString);
    std::string line;

    Timetable::ClassContainer classes;

    while (std::getline(ss, line)) {
        Class clazz = Class::deserialize(line);
        classes.push_back(clazz);
    }

    return Timetable(classes);
}