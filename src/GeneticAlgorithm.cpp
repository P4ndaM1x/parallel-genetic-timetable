#include "GeneticAlgorithm.hpp"
#include "CLIManger.hpp"
#include "Chromosome.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>

GeneticAlgorithm::GeneticAlgorithm(Timetable& timetable, const unsigned populationSize, const unsigned numberOfGenerations, const double mutationRate)
    : solution { timetable }
    , populationSize { populationSize }
    , numberOfGenerations { numberOfGenerations }
    , mutationRate { mutationRate }
{
    for (unsigned i = 0; i < populationSize; i++) {
        this->population.push_back(Chromosome { timetable.getClasses() });
    }
}

std::vector<Chromosome> GeneticAlgorithm::getPopulation()
{
    return this->population;
}

void GeneticAlgorithm::initialize()
{
    if (CLI::Args::verbose) {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    std::for_each(std::begin(population), std::end(population), [](Chromosome& c) { c.init(); });
}

void GeneticAlgorithm::evolve()
{
    if (CLI::Args::verbose) {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    for (unsigned i = 0; i < numberOfGenerations; ++i) {
        fitness();
        selectBest();
        crossover();
        mutate();
    }
}

void GeneticAlgorithm::fitness()
{
    std::for_each(std::begin(population), std::end(population), [](Chromosome& c) { c.calculateError(); });
}

void GeneticAlgorithm::selectBest()
{
    std::sort(std::begin(population), std::end(population), [](const Chromosome& a, const Chromosome& b) { return a.getError() < b.getError(); });
    population.erase(std::begin(population) + populationSize * percentToKeep, std::end(population));
    population.reserve(populationSize);
}

void GeneticAlgorithm::mutate()
{
    for (auto& c : population) {
        if ((static_cast<double>(std::rand()) / RAND_MAX) < mutationRate) {
            c.mutate();
        }
    }
}

void GeneticAlgorithm::crossover()
{
    ChrosomeContainer parents { population };
    for (unsigned i = population.size(); i < populationSize; ++i) {
        const auto& firstParent = parents.at(std::rand() % parents.size());
        const auto& secondParent = parents.at(std::rand() % parents.size());
        auto child = makeLove(firstParent, secondParent);
        population.push_back(child);
    }
    assert(population.size() == populationSize);
}

Chromosome GeneticAlgorithm::makeLove(const Chromosome& a, const Chromosome& b)
{
    Timetable::ClassContainer newClasses;
    for (auto& c : solution.getClasses()) {
        const auto& classA = a.getClass(c.getId());
        const auto& classB = b.getClass(c.getId());
        const auto& classToUse = std::rand() % 2 == 0 ? classA : classB;
        newClasses.push_back(classToUse);
    }
    return Chromosome { newClasses };
}

void GeneticAlgorithm::run()
{
    if (CLI::Args::verbose) {
        std::cout << "Running Genetic Algorithm" << std::endl;
    }
    initialize();
    evolve();
}
