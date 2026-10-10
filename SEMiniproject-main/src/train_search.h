#ifndef TRAIN_SEARCH_H
#define TRAIN_SEARCH_H

#include "models.h"
#include <string>
#include <vector>

// ============================================================================
// Train Search Module (teammate: Parinitha / R Vinay)
// Maps to: FR-001 through FR-009
// ============================================================================

// Search for trains by source and destination
// Maps to: FR-001, FR-002, FR-003, FR-004, FR-005
std::vector<Train> searchTrains(const std::vector<Train>& trains,
                                 const std::string& source,
                                 const std::string& destination);

// Display a list of matching trains
void displayTrainList(const std::vector<Train>& matchingTrains);

// Display detailed info for a single train
// Maps to: FR-006, FR-007, FR-008, FR-009
void displayTrainDetails(const Train& train);

// Interactive search flow
void searchTrainFlow(const std::vector<Train>& trains);

// Check seat availability for a specific train
// Maps to: FR-009, FR-013
void checkSeatAvailabilityFlow(const std::vector<Train>& trains);

#endif // TRAIN_SEARCH_H
