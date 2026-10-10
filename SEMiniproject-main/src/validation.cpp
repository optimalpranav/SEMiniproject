#include "validation.h"
#include <algorithm>
#include <cctype>
#include <sstream>

bool isValidReservationId(const std::string& id) {

    if (id.empty()) return false;
    
    if (id.length() > 20) return false;

    if (id.length() < 4 || id.substr(0, 3) != "RES") return false;

    for (size_t i = 3; i < id.length(); i++) {

        if (!std::isdigit(static_cast<unsigned char>(id[i]))) return false;

    }

    return true;

}

bool isValidPassengerName(const std::string& name) {

    if (name.empty()) return false;

    if (name.length() > 100) return false;

    for (char c : name) {

        if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ') {

            return false;

        }

    }

    bool hasAlpha = false;

    for (char c : name) {

        if (std::isalpha(static_cast<unsigned char>(c))) {

            hasAlpha = true;

            break;

        }

    }

    return hasAlpha;

}

bool isValidAge(int age) {

    return (age >= 1 && age <= 120);

}

bool isValidDate(const std::string& date) {

    if (date.length() != 10) return false;

    if (date[2] != '-' || date[5] != '-') return false;

    for (int i = 0; i < 10; i++) {

        if (i == 2 || i == 5) continue;

        if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;

    }

    int day = std::stoi(date.substr(0, 2));

    int month = std::stoi(date.substr(3, 2));

    int year = std::stoi(date.substr(6, 4));

    if (month < 1 || month > 12) return false;

    if (day < 1 || day > 31) return false;

    if (year < 2024 || year > 2030) return false;

    return true;

}

bool isValidGender(const std::string& gender) {

    return (gender == "M" || gender == "F" || gender == "Other");

}

bool isValidStation(const std::string& station) {

    if (station.empty()) return false;

    if (station.length() > 50) return false;

    for (char c : station) {

        if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ') {

            return false;

        }

    }

    return true;

}

std::string sanitizeForDisplay(const std::string& input, int maxLength) {

    std::string sanitized;

    int count = 0;

    for (char c : input) {

        if (count >= maxLength) {

            sanitized += "...";

            break;

        }

        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == ' ') {

            sanitized += c;

        } else {

            sanitized += '?';

        }

        count++;

    }

    return sanitized;

}