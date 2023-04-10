#include "globals.hpp"

char addressBook[255][100]; // A 2D array to store node names
SemaphoreHandle_t addressBookMutex;