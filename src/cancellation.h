#ifndef CANCELLATION_H
#define CANCELLATION_H
#include "models.h"
#include <string>
#include <vector>
CancelResult processCancellation(const std::string& reservationId,
                                  std::vector<Reservation>& reservations,
                                  std::vector<Train>& trains);
bool updateSeatAvailability(std::vector<Train>& trains,
                             const std::string& trainId,
                             int seatsToRelease);
CancelResult cancelReservationFlow(std::vector<Reservation>& reservations,
                                    std::vector<Train>& trains);
#endif 