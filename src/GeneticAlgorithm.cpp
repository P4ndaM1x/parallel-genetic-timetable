#include "GeneticAlgorithm.hpp"


GeneticAlgorithm::GeneticAlgorithm(Timetable& timetable, const unsigned populationSize, const unsigned numberOfGenerations, const double mutationRate)
    : solution { timetable }
    , populationSize { populationSize }
    , numberOfGenerations { numberOfGenerations }
    , mutationRate { mutationRate }
{
    for(int i = 0; i < populationSize; i++)
    {
        this->population.push_bash(Chromosome(timetable.getClasses()));
    }
}

std::vector<Chromosome> GeneticAlgorithm::getPopulation()
{
    return this->population;
}

void GeneticAlgorithm::initialize()
{
}

void GeneticAlgorithm::evolve()
{
}

void GeneticAlgorithm::fitness()
{
}

void GeneticAlgorithm::mutate()
{
}

void GeneticAlgorithm::crossover()
{
}

void GeneticAlgorithm::run()
{
}
