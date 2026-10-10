#ifndef DATA_STORE_H
#define DATA_STORE_H
#include "models.h"
#include <string>
#include <vector>
std::vector<Train> loadTrains(const std::string& filepath = "data/trains.dat");
bool saveTrains(const std::vector<Train>& trains, const std::string& filepath = "data/trains.dat");
Train* findTrainById(std::vector<Train>& trains, const std::string& trainId);
std::vector<Reservation> loadReservations(const std::string& filepath = "data/reservations.dat");
bool saveReservations(const std::vector<Reservation>& reservations, const std::string& filepath = "data/reservations.dat");
Reservation* findReservationById(std::vector<Reservation>& reservations, const std::string& reservationId);
std::string generateReservationId(const std::vector<Reservation>& reservations);
#endif 