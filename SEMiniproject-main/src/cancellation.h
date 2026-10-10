#ifndef CANCELLATION_H
#define CANCELLATION_H

#include "models.h"
#include <string>
#include <vector>

// ============================================================================
// Ticket Cancellation Module
// Handles the full cancellation workflow: lookup → display → confirm →
// update status → update seat availability.
// Maps to: FR-024 through FR-034
// Business Rules: BR-004, BR-005, BR-006
// Security: SEC-001, SEC-002, SEC-004, SEC-005
// Architecture: Application/Business Layer (SAD Section 3.5)
// ============================================================================

// Cancel a reservation (non-interactive, for testability)
// Performs validation, status check, and updates.
// Parameters:
//   reservationId  - The reservation reference to cancel
//   reservations   - Vector of all reservations (will be modified on success)
//   trains         - Vector of all trains (seats will be updated on success)
// Returns: CancelResult code
CancelResult processCancellation(const std::string& reservationId,
                                  std::vector<Reservation>& reservations,
                                  std::vector<Train>& trains);

// Update available seats for a train after cancellation
// Maps to: FR-030, BR-006
// Parameters:
//   trains         - Vector of all trains
//   trainId        - ID of the train whose seats should be restored
//   seatsToRelease - Number of seats to add back
// Returns: true if train found and seats updated, false otherwise
bool updateSeatAvailability(std::vector<Train>& trains,
                             const std::string& trainId,
                             int seatsToRelease);

// Interactive cancellation flow (full user workflow)
// Maps to: FR-024 through FR-034
// Prompts user for reservation ID, displays details, asks for confirmation,
// and performs the cancellation with seat update.
// Returns: CancelResult code
CancelResult cancelReservationFlow(std::vector<Reservation>& reservations,
                                    std::vector<Train>& trains);

#endif // CANCELLATION_H
