#ifndef AGENT_H
#define AGENT_H

#include "core.h"

/**
 * This function takes as input the status of the game, then:
 * Status -> Kohonen Network -> Q_Matrix -> Action
 */
ActionMask GetActionFromAgent(Lander lander);

#endif //AGENT_H