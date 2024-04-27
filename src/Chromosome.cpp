#include "Chromosome.hpp"
#include "Class.hpp"
#include "Timetable.hpp"

#include <cstdlib>

Chromosome::Chromosome(const Timetable::ClassContainer& classes)
    : classes { classes }
{
    this->init();
}

void Chromosome::printSolution()
{
    std::ostringstream stringBuffers[Timetable::numberOfSlots];

    for (unsigned i = 0; i < Timetable::numberOfSlots; i++) {
        stringBuffers[i] << i;
        stringBuffers[i] << ". ";
    }

    for (unsigned i = 0; i < Timetable::numberOfSlots; i++) {

        TimeSlot timeSlot = this->timeSlots[i];
        for (unsigned j = 0; j < timeSlot.size(); j++) {
            stringBuffers[i] << "[";
            stringBuffers[i] << timeSlot[j];
            stringBuffers[i] << "]";
        }
    }

    std::cout << "-----------------------------";
    for (unsigned i = 0; i < Timetable::numberOfSlots; ++i) {
        if (i % Timetable::slotsPerDay == 0) {
            std::cout << "-----------------------------" << std::endl;
        }
        std::cout << stringBuffers[i].str() << std::endl;
    }
}

void Chromosome::init()
{
    for (auto& c : this->classes) {
        do {
            c.setStartTime(std::rand() % (Timetable::numberOfSlots - c.getDurationTime()));
        } while (!this->isIntervalValid(c.getStartTime(), c.getEndTime()));

        for (auto i = c.getStartTime(); i < c.getEndTime(); i++) {
            timeSlots.at(i).push_back(c.getId());
        }
    }
}

bool Chromosome::isIntervalValid(Class::Time a, Class::Time b)
{
    return (a / Timetable::slotsPerDay) == ((b - 1) / Timetable::slotsPerDay);
}

void Chromosome::mutate()
{
}

int Chromosome::calculateFitness()
{
    return 0;
}

int Chromosome::getFitness()
{
    return this->fitness;
}
