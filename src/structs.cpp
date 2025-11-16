

#include "structs.h"


Property properties[MAX_PROPERTIES];


int propertyCount = 0;


const char* getStatusString(enum Status status) {
    switch (status) {
        case SOLD:
            return "Sold";
        case RESERVED:
            return "Reserved";
        case AVAILABLE:
            return "Available";
        default:
            return "Unknown";
    }
}