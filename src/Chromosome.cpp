#include "Chromosome.hpp"

#include <cstdlib>

Chromosome::Chromosome(const std::vector<Class>& classes)
    : classes { classes }
{
}

void Chromosome::printSolution()
{
    std::ostringstream stringBuffers[Timetable::NumberOfSlots];

    for(int i = 0; i < Timetable::NumberOfSlots; i++)
    {
        TimeSlot timeSlot = this->timeslots[i];
        for(int j = 0; j < timeSlot.length(); j++)
        {
            stringBuffers[i+j] << timeSlot[j];
        }
    }

    for (int i = 0; i < Timetable::NumberOfSlots; ++i) {
        std::cout << buffers[i].str() << std::endl;
    }
}

void Chromosome::randomize()
{
    for (auto& c : classes) {
        c.setStartingHour(std::rand() % Timetable::numberOfSlots);
        for (auto i = c.getStartingHour(); i < c.getEndingHour(); ++i) {
            timeSlots.at(i).push_back(c.getId());
        }
    }
}

void Chromosome::mutate()
{
}

int Chromosome::calculateFitness()
{
}

int Chromosome::getFitness()
{
    return this->fitness;
}
