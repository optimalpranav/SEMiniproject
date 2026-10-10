#ifndef VALIDATION_H
#define VALIDATION_H
#include <string>
bool isValidReservationId(const std::string& id);
bool isValidPassengerName(const std::string& name);
bool isValidAge(int age);
bool isValidDate(const std::string& date);
bool isValidGender(const std::string& gender);
bool isValidStation(const std::string& station);
std::string sanitizeForDisplay(const std::string& input, int maxLength = 20);
#endif 