#include "Chromosome.hpp"
#include "Class.hpp"
#include "Timetable.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>

Chromosome::Chromosome(const Timetable::ClassContainer& classes, const bool randomizeStartTimes)
    : classes { classes }
{
    if (randomizeStartTimes) {
        init();
    } else {
        updateTimeSlotContainer();
    }
}

void Chromosome::printSolution() const
{
    std::ostringstream stringBuffers[Timetable::numberOfSlots];

    for (unsigned i = 0; i < Timetable::numberOfSlots; i++) {
    }

    for (unsigned i = 0; i < Timetable::numberOfSlots; i++) {

        stringBuffers[i] << std::setw(std::log10(Timetable::numberOfSlots)+1) << i << ". ";
        TimeSlot timeSlot = timeSlots[i];
        for (unsigned j = 0; j < timeSlot.size(); j++) {
            stringBuffers[i] << "[";
            stringBuffers[i] << timeSlot[j];
            stringBuffers[i] << "]";
        }
    }

    std::cout << std::endl
              << std::endl
              << "Timetable";
    constexpr auto spacer = "------------------------";
    for (unsigned i = 0; i < Timetable::numberOfSlots; ++i) {
        if (i % Timetable::slotsPerDay == 0) {
            std::cout << std::endl
                      << spacer;
        }
        std::cout << std::endl
                  << stringBuffers[i].str();
    }
    std::cout << std::endl
              << spacer << std::endl;
}

void Chromosome::updateTimeSlotContainer()
{
    std::for_each(std::begin(timeSlots), std::end(timeSlots), [](auto& ts) { ts.clear(); });
    for (auto& c : classes) {
        for (auto i = c.getStartTime(); i < c.getEndTime(); i++) {
            timeSlots.at(i).push_back(c.getId());
        }
    }
}
void Chromosome::init()
{
    std::for_each(std::begin(classes), std::end(classes), [](auto& c) { c.setRandomStartTime(); });
    updateTimeSlotContainer();
}

bool Chromosome::isIntervalValid(Class::Time a, Class::Time b)
{
    return (a / Timetable::slotsPerDay) == ((b - 1) / Timetable::slotsPerDay);
}

void Chromosome::mutate()
{
    classes.at(std::rand() % classes.size()).setRandomStartTime();
    updateTimeSlotContainer();
}

uint32_t Chromosome::calculateError()
{
    error = 0;
    for (auto& slot : timeSlots) {
        slot.size() > 1 ? error += slot.size() - 1 : 0;
    }
    return 0;
}

uint32_t Chromosome::getError() const
{
    return error;
}

Class Chromosome::getClass(const Class::ID classID) const
{
    auto it = std::find_if(std::begin(classes), std::end(classes), [classID](auto& c) { return classID == c.getId(); });
    if (it == std::end(classes)) {
        throw std::runtime_error("Class not found");
    }
    return *it;
}

Timetable::ClassContainer Chromosome::getClasses() const
{
    return classes;
}
