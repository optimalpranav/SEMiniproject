#ifndef RESERVATION_H
#define RESERVATION_H

#include "models.h"
#include <string>
#include <vector>

// ============================================================================
// Reservation Lookup Module
// Handles looking up and displaying reservation details.
// Maps to: FR-019, FR-020, FR-021, FR-022, FR-023
// Security: SEC-001, SEC-003
// Architecture: Application/Business Layer (SAD Section 3.5)
// ============================================================================

// Look up a reservation by its ID
// Validates the ID, searches the data store, and returns the result.
// Parameters:
//   reservationId  - The reservation reference entered by the user
//   reservations   - Vector of all reservations (loaded from data store)
//   outReservation - Output: populated with reservation data if found
// Returns: LookupResult code
LookupResult lookupReservation(const std::string& reservationId,
                                std::vector<Reservation>& reservations,
                                Reservation& outReservation);

// Display reservation details to the console
// Shows: reservation ID, train info, passenger info, journey date,
//        seats booked, and current status.
// Parameters:
//   reservation - The reservation to display
//   trains      - Vector of trains (to resolve train name from ID)
void displayReservation(const Reservation& reservation,
                        const std::vector<Train>& trains);

// Display reservation status as a human-readable string
// Maps to: FR-023
std::string getStatusString(ReservationStatus status);

// Interactive view reservation flow
// Prompts user for reservation ID, looks it up, and displays details.
// Maps to: FR-019 through FR-023
// Returns: true if reservation was found and displayed, false otherwise
bool viewReservationFlow(std::vector<Reservation>& reservations,
                         const std::vector<Train>& trains);

#endif // RESERVATION_H
