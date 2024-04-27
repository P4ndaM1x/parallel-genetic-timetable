#include "Chromosome.hpp"
#include "Class.hpp"

#include <cstdlib>

Chromosome::Chromosome(const Timetable& timetable): timetable{timetable}
{
    this->init(timetable.getClasses());
}

void Chromosome::printSolution()
{
    std::ostringstream stringBuffers[Timetable::numberOfSlots];

    for(unsigned i = 0; i < Timetable::numberOfSlots; i++)
    {
        stringBuffers[i] << i;
        stringBuffers[i] << ". ";
    }

    for(unsigned i = 0; i < Timetable::numberOfSlots; i++)
    {

        TimeSlot timeSlot = this->timeSlots[i];
        for(unsigned j = 0; j < timeSlot.size(); j++)
        {
            stringBuffers[i+j] << "[";
            stringBuffers[i+j] << timeSlot[j];
            stringBuffers[i+j] << "]";
        }
    }

    std::cout << "-----------------------------";
    for (unsigned i = 0; i < Timetable::numberOfSlots; ++i) {
        if(i % Timetable::slotsPerDay == 0)
        {
            std::cout << "-----------------------------" << std::endl;
        }
        std::cout << stringBuffers[i].str() << std::endl;
    }
}

void Chromosome::init(const std::vector<Class>& classes)
{
    for (const auto& c : classes) {
        Class::Time start = std::rand() % (Timetable::numberOfSlots - c.getDurationTime());
        Class::Time end = start + c.getDurationTime();
        std::cout << (int) start << " " << (int) end << " " << (int) c.getDurationTime()<< "\n";
        for (auto i = start; i < end; i++) {
            timeSlots.at(i).push_back(c.getId());
        }
    }
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
