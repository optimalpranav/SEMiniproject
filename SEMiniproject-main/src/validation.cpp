#include "validation.h"
#include <algorithm>
#include <cctype>
#include <sstream>

// ============================================================================
// Input Validation Implementation
// Maps to: FR-035, FR-036, FR-037, FR-038, FR-039
// Security: SEC-001, SEC-003, SEC-004
// ============================================================================

bool isValidReservationId(const std::string& id) {
    // Must not be empty (FR-036)
    if (id.empty()) return false;

    // Must not exceed reasonable length (prevents buffer issues)
    if (id.length() > 20) return false;

    // Must start with "RES" prefix
    if (id.length() < 4 || id.substr(0, 3) != "RES") return false;

    // Remaining characters must be digits
    for (size_t i = 3; i < id.length(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(id[i]))) return false;
    }

    return true;
}

bool isValidPassengerName(const std::string& name) {
    // Must not be empty (FR-037)
    if (name.empty()) return false;

    // Must not exceed reasonable length
    if (name.length() > 100) return false;

    // Must contain only alphabetic characters and spaces
    for (char c : name) {
        if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ') {
            return false;
        }
    }

    // Must have at least one alphabetic character
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
    // Format: DD-MM-YYYY
    if (date.length() != 10) return false;

    if (date[2] != '-' || date[5] != '-') return false;

    // Check that all other characters are digits
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
    }

    // Basic range validation
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
    // SEC-003: Prevent sensitive data from leaking in error messages
    std::string sanitized;
    int count = 0;

    for (char c : input) {
        if (count >= maxLength) {
            sanitized += "...";
            break;
        }
        // Only keep alphanumeric characters and basic punctuation
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == ' ') {
            sanitized += c;
        } else {
            sanitized += '?';
        }
        count++;
    }

    return sanitized;
}
