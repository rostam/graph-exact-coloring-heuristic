//
// Created by rostami on 28.06.17.
//

#ifndef EXACT_COLORING_GENERATORINTERFACE_H
#define EXACT_COLORING_GENERATORINTERFACE_H

#include "../datatypes.h"

class GeneratorInterface {
    virtual Graph generate() = 0;
};


#endif //EXACT_COLORING_GENERATORINTERFACE_H
