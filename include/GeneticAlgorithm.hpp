/**
 * @file GeneticAlgorithm.hpp
 * @brief Header file for the GeneticAlgorithm class.
 * @see GeneticAlgorithm.cpp
 */
#pragma once

#include "Chromosome.hpp"
#include "Timetable.hpp"

#include <vector>

class GeneticAlgorithm {

public:
    using ChrosomeContainer = std::vector<Chromosome>;

    GeneticAlgorithm(
        Timetable& timetable,
        const unsigned populationSize,
        const unsigned numberOfGenerations,
        const double mutationRate
    );

    // entry point for the algorithm
    void run();
    std::vector<Chromosome> getPopulation();

private:
    // Initialize the population with random chromosomes
    void initialize();
    // Perform the whole iteration of the algorithm
    void evolve();
    // Calculate the fitness of the current population
    void fitness();
    // Randomly select a class and change its starting time
    void mutate();
    // Perform the crossover operation
    void crossover();

    Chromosome makeLove(const Chromosome& a, const Chromosome& b);

    void selectBest();

    Timetable& solution;
    const unsigned populationSize;
    const unsigned numberOfGenerations;
    const double mutationRate;
    const double percentToKeep{0.1};

    ChrosomeContainer population;
};
