#include <iostream>
#include <string>

double celsius_to_fahrenheit(double c_temp) {
    return ((9.0 / 5.0) * c_temp) + 32;
}

double meters_to_feet(double meters) {
    return meters * 3.28084;
}

double bytes_to_kilobytes(double bytes) {
    return bytes / 1024.0;
}

int main() {

    std::cout << "===Unit Converter===\n";
    std::cout << "1. Celsuius to Fahrenheit\n";
    std::cout << "2. Meters to Feet\n";
    std::cout << "3. Bytes to Kilobytes\n";

    int choice;

    std::cout << "Choose and option: ";
    std::cin >> choice;

    double value;
    std::cout << "Enter the value to be converted: ";
    std::cin >> value;

    if (choice == 1) {
        double fahrenheit = celsius_to_fahrenheit(value);
        std::cout << fahrenheit << " degrees F\n";
    } else if (choice == 2) {
        double feet = meters_to_feet(value);
        std::cout << feet << " feet\n";
    } else if (choice == 3) {
        double kilo = bytes_to_kilobytes(value);
        std::cout << kilo << " kilobytes\n";
    } else {
        std::cout << "Invalid choice!\n";
    }

    return 0;

}
