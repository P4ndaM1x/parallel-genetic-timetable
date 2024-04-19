#include "Chromosome.hpp"

#include <cstdlib>

Chromosome::Chromosome(const Timetable::data_type& classes)
    : classes { classes }
{
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

void Chromosome::fitness()
{
}
