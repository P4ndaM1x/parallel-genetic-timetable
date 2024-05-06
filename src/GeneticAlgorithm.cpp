#include "GeneticAlgorithm.hpp"
#include "Chromosome.hpp"
#include "Log.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>

GeneticAlgorithm::GeneticAlgorithm(
    const Timetable& timetable,
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
    initialize();
}

GeneticAlgorithm::ChromosomeContainer& GeneticAlgorithm::getPopulation() { return population; }

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
        sortPopulationByError();
        selectBest();
        crossover();
        mutate();
    }
}

void GeneticAlgorithm::fitness()
{
    std::ranges::for_each(population, [](Chromosome& c) { c.calculateError(); });
}

void GeneticAlgorithm::sortPopulationByError()
{
    std::ranges::sort(population, [](const Chromosome& a, const Chromosome& b) {
        return a.getError() < b.getError();
    });
}

void GeneticAlgorithm::selectBest()
{
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
    ChromosomeContainer parents{population};
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

GeneticAlgorithm::ChromosomeContainer& GeneticAlgorithm::step()
{
    Log::print("Running genetic algorithm step...");
    evolve();
    Log::print("Saving solution...");
    solution.updateClasses(population.at(0).getClasses());
    return population;
}

std::string GeneticAlgorithm::serializePopulation(const ChromosomeContainer& population)
{
    std::stringstream ss;
    for (const auto& chromosome : population) {
        ss << chromosome.serialize() << "\n";
    }
    std::string serializedPopulation = ss.str();
    serializedPopulation.pop_back();
    return serializedPopulation;
}

std::string GeneticAlgorithm::serializePopulationOfSize(unsigned size)
{
    sortPopulationByError();
    GeneticAlgorithm::ChromosomeContainer bestPop(population.begin(), population.begin() + size);
    return serializePopulation(bestPop);
}

GeneticAlgorithm::ChromosomeContainer
GeneticAlgorithm::deserializePopulation(std::string serializedPopulation)
{
    std::stringstream iss(serializedPopulation);
    std::string line;

    GeneticAlgorithm::ChromosomeContainer deserializedPopulation;
    while (std::getline(iss, line)) {
        if (line.empty()) {
            continue;
        }
        deserializedPopulation.push_back(Chromosome::deserialize(line));
    }
    return deserializedPopulation;
}

void GeneticAlgorithm::setPopulation(const ChromosomeContainer& updatedPopulation)
{
    this->population = updatedPopulation;
}

GeneticAlgorithm::ChromosomeContainer GeneticAlgorithm::getBestPopulationOfSize(unsigned size)
{
    fitness();
    sortPopulationByError();
    return GeneticAlgorithm::ChromosomeContainer(population.begin(), population.begin() + size);
}