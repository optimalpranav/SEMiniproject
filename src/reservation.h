#ifndef RESERVATION_H
#define RESERVATION_H
#include "models.h"
#include <string>
#include <vector>
LookupResult lookupReservation(const std::string& reservationId,
                                std::vector<Reservation>& reservations,
                                Reservation& outReservation);
void displayReservation(const Reservation& reservation,
                        const std::vector<Train>& trains);
std::string getStatusString(ReservationStatus status);
bool viewReservationFlow(std::vector<Reservation>& reservations,
                         const std::vector<Train>& trains);
#endif 