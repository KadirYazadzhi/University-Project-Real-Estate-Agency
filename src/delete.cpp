#include <iostream>


using namespace std;


void DeleteProperty() {
    cout << CYAN << "Enter the reference number of the property you wish to delete: " << RESET;
    int refNumber;
    cin >> refNumber;

    int index = getPropertyByRefNumber(refNumber);
    if (index == -1) {
        cout << RED << "No property exists with this reference number." << RESET << endl;

        return;
    }

    cout << YELLOW << "Are you sure you want to delete property with ref. number - " << refNumber << ". [y/n]: " << RESET;
    char option;
    cin >> option;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (tolower(option) != 'y') {
        cout << YELLOW << "Deletion cancelled." << RESET << endl;
        return;
    }


    for (int i = index; i < propertyCount - 1; i++) {
        properties[i] = properties[i + 1];
    }

    cout << GREEN << "Property with ref. number " << refNumber << " was successfully deleted." << RESET << endl;




}


void DeleteAllProperties() {
    if (propertyCount == 0) {
        cout << YELLOW << "No properties to delete." << RESET << endl;
        return;
    }

    cout << YELLOW << "Are you sure you want to delete ALL properties? This operation is irreversible. [y/n]: " << RESET;
    char option;
    cin >> option;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (tolower(option) != 'y') {
        cout << YELLOW << "Deletion of all properties cancelled." << RESET << endl;
        return;
    }



    cout << GREEN << "All properties were successfully deleted." << RESET << endl;

}