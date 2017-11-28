
//#define DEBUGBOT_GRAPH

#ifdef DEBUGBOT_GRAPH
#define DEBUGBOT

#endif // DEBUGBOT_GRAPH

#ifdef DEBUGBOT
#include <fstream>
using namespace std;
ofstream file{ "sortie.txt", ios::app };
#endif // DEBUGBOT
