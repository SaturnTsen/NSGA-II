#pragma once

#include "individual.h"
#include "nsga2.h"

namespace nsga2 {
    /** A wrapper over `individual_t` that contains crowd neighbor metadata. */
    struct CrowdIndividualWrapper {

    };

    class ModifiedNSGA2 : private NSGA2 {
        population_t crowding_distance_select(population_t &population, fronts_t &fronts);
    };
}