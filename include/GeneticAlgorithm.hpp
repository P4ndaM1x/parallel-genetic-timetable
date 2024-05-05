#pragma once

#include "Chromosome.hpp"
#include "Timetable.hpp"

#include <vector>

class GeneticAlgorithm {

public:
    using ChromosomeContainer = std::vector<Chromosome>;

    GeneticAlgorithm(
        Timetable& timetable,
        const unsigned populationSize,
        const unsigned numberOfGenerations,
        const double mutationRate
    );

    // entry point for the algorithm
    ChromosomeContainer& step();
    ChromosomeContainer& getPopulation();
    void setPopulation(const ChromosomeContainer& updatedPopulation);
    void initialize();
    std::string serializePopulationOfSize(unsigned size);
    ChromosomeContainer getTopPopulationOfSize(unsigned size);

    static ChromosomeContainer deserializePopulation(std::string serializedPopulation);
    static std::string serializePopulation(const ChromosomeContainer& population);

private:
    // Perform the whole iteration of the algorithm
    void evolve();
    // Calculate the fitness of the current population
    void fitness();
    // Randomly select a class and change its starting time
    void mutate();
    // Perform the crossover operation
    void crossover();

    Chromosome makeLove(const Chromosome& a, const Chromosome& b);

    void sortPopulationByError();
    void selectBest();

    Timetable& solution;
    const unsigned populationSize;
    const unsigned numberOfGenerations;
    const double mutationRate;
    const double percentToKeep{0.1};

    ChromosomeContainer population;
};
