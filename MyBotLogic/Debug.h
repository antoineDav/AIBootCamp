#pragma once

//#define DEBUGBOT_GRAPH
//#define DEBUGBOT_TREE
//#define DEBUGBOT_MISSION

#ifdef DEBUGBOT_GRAPH
#define DEBUGBOT
#endif // DEBUGBOT_GRAPH

#ifdef DEBUGBOT_TREE
#define DEBUGBOT
#endif // DEBUGBOT_TREE


#ifdef DEBUGBOT_MISSION
#define DEBUGBOT
#endif // DEBUGBOT_MISSION

