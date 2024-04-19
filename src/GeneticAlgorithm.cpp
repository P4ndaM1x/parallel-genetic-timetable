#include "GeneticAlgorithm.hpp"


GeneticAlgorithm::GeneticAlgorithm(Timetable& timetable, const unsigned populationSize, const unsigned numberOfGenerations, const double mutationRate)
    : solution { timetable }
    , populationSize { populationSize }
    , numberOfGenerations { numberOfGenerations }
    , mutationRate { mutationRate }
{
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
