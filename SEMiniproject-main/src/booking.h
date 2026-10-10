#ifndef BOOKING_H
#define BOOKING_H

#include "models.h"
#include <string>
#include <vector>

// ============================================================================
// Ticket Booking Module (teammate: Dhriti / Parinitha)
// Maps to: FR-010 through FR-018
// ============================================================================

// Interactive booking flow
// Maps to: FR-010 through FR-018
// Returns: true if booking was successful
bool bookTicketFlow(std::vector<Train>& trains,
                     std::vector<Reservation>& reservations);

#endif // BOOKING_H
