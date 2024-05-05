/**
 * @file GeneticAlgorithm.cpp
 * @brief Implementation file of the GeneticAlgorithm class.
 */
#include "GeneticAlgorithm.hpp"

#include "Chromosome.hpp"
#include "Log.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>

GeneticAlgorithm::GeneticAlgorithm(
    Timetable& timetable,
    const unsigned populationSize,
    const unsigned numberOfGenerations,
    const double mutationRate
)
    : solution{timetable}
    , populationSize{populationSize}
    , numberOfGenerations{numberOfGenerations}
    , mutationRate{mutationRate}
{
    for (unsigned i = 0; i < populationSize; i++) {
        population.push_back(Chromosome{timetable.getClasses()});
    }
}

std::vector<Chromosome> GeneticAlgorithm::getPopulation() { return population; }

void GeneticAlgorithm::initialize()
{
    Log::print("Initializing population...", Severity::DEBUG);
    std::ranges::for_each(population, [](Chromosome& c) { c.init(); });
}

void GeneticAlgorithm::evolve()
{
    Log::print("Starting evolution...");

    for (unsigned i = 1; i <= numberOfGenerations; ++i) {

        const auto logMessage
            = "Generation " + std::to_string(i) + " of " + std::to_string(numberOfGenerations);
        Log::print(logMessage, Severity::DEBUG, true);

        const unsigned logFrequency = numberOfGenerations * 0.01;
        if (i % (numberOfGenerations / logFrequency) == 0) {
            Log::print(logMessage);
        }

        fitness();
        selectBest();
        crossover();
        mutate();
    }
}

void GeneticAlgorithm::fitness()
{
    std::ranges::for_each(population, [](Chromosome& c) { c.calculateError(); });
}

void GeneticAlgorithm::selectBest()
{
    std::ranges::sort(population, [](const Chromosome& a, const Chromosome& b) {
        return a.getError() < b.getError();
    });
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
    ChrosomeContainer parents{population};
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
    return Chromosome{newClasses};
}

void GeneticAlgorithm::run()
{
    Log::print("Running genetic algorithm...");
    initialize();
    evolve();
    Log::print("Saving solution...");
    solution.updateClasses(population.at(0).getClasses());
}
