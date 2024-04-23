

#include <iostream>
#include<fstream>
#include<string>
#include <iomanip>
#include <ctime>
#include <sstream> 
#include <cstdlib>
#include "sha1.hpp"
//#include <openssl/sha.h>
using namespace std;


// Function to convert text file to PDF using Pandoc
void convertToPDF(const char* textFileName, const char* pdfFileName) {
    // Check if the file exists
    std::ifstream file(textFileName);
    if (!file.is_open()) {
        std::cerr << "Error: File " << textFileName << " does not exist or cannot be opened." << std::endl;
        return;
    }
    file.close();
    
    // Construct the Pandoc command with quotes around filenames
    std::string command = "pandoc \"";
    command += textFileName;
    command += "\" -o \"";
    command += pdfFileName;
    command += "\"";

    // Print out the command
    std::cout << "Command: " << command << std::endl;

    // Execute the command
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error: Conversion to PDF failed." << std::endl;
    }
}

// Function to compile LaTeX document
bool compileLatex(const std::string& latexContent, const std::string& latexFilePath) {
    // Write LaTeX content to file
    std::ofstream latexFile(latexFilePath);
    if (!latexFile.is_open()) {
        std::cerr << "Error: Failed to open LaTeX file for writing" << std::endl;
        return false;
    }
    latexFile << latexContent;
    latexFile.close();

    // Compile LaTeX document to PDF
    std::string command = "pdflatex -interaction=batchmode " + latexFilePath;
    int exitCode = std::system(command.c_str());
    if (exitCode != 0) {
        std::cerr << "Error: Failed to compile LaTeX document" << std::endl;
        return false;
    }

    return true;
}
// Date structure
struct Date {
    string startDate;
    string endDate;
};

// Car structure
struct Car {
    string plateNumber;
    string brand;
    string model;
    int year;
    string color;
    double pricePerDay;
    Date d;
};

// Client structure
struct Client {
    int userID;
    string firstName;
    string lastName;
    string password;
    string phone;
    string email;
    int nbReservation;
    Car* c; 
};

struct Reservation {
    int userID;
    string plateNumber;
    double pricePerDay;
    string startDate;
    string endDate;
};


bool verifyPassword(string password)
{
    if (password.length() < 8) {
        return false;
    }

    bool hasDigit = false;
    bool hasLetter = false;
    bool hasSpecialChar = false;

    for (char c : password) {

        if (isdigit(c)) {
            hasDigit = true;
        }

        else if (isalpha(c)) {
            hasLetter = true;
        }

        else if (ispunct(c)) {
            hasSpecialChar = true;
        }
    }

    return hasDigit && hasLetter && hasSpecialChar;

}
string addDaysToDate(string startDate, int nbOfDays) {
    // Convert string date to tm struct
    tm date = {};
    stringstream ss(startDate);
    ss >> get_time(&date, "%Y-%m-%d");

    // Add number of days to date
    date.tm_mday += nbOfDays;
    mktime(&date); // Normalize the date

    // Convert updated tm struct back to string
    stringstream ss2;
    ss2 << put_time(&date, "%Y-%m-%d");
    return ss2.str();
}
bool verifyEmail(string email)
{
    // Check if email contains '@' character
    size_t atPos = email.find('@');
    if (atPos == string::npos) {
        return false; 
    }

    // Check if there is at least one character before '@'
    if (atPos == 0) {
        return false;
    }

    // Check if there is at least one character after '@'
    if (atPos == email.length() - 1) {
        return false; 
    }

    // Check if there is only one '@' character
    size_t nextAtPos = email.find('@', atPos + 1);
    if (nextAtPos != string::npos) {
        return false; 
    }

    // Check if the domain part contains at least one '.' character
    size_t dotPos = email.find('.', atPos + 1);
    if (dotPos == string::npos || dotPos == email.length() - 1) {
        return false; 
    }

    // Check if there is at least one character before the first '.'
    if (dotPos == atPos + 1) {
        return false; // No characters before the first '.'
    }

    // Check if there is at least one character after the last '.'
    if (dotPos == email.length() - 2) {
        return false; 
    }

    return true; 
}

bool verifyPhone(string phone) {
    // Check if the length is exactly 8 characters
    if (phone.length() != 9) {
        return false;
    }

    // Check if the first two characters are digits
    if (!isdigit(phone[0]) || !isdigit(phone[1])) {
        return false;
    }

    // Check if the third character is '-'
    if (phone[2] != '-') {
        return false;
    }

    // Check if the remaining characters are digits
    for (size_t i = 3; i < phone.length(); ++i) {
        if (!isdigit(phone[i])) {
            return false;
        }
    }

    return true;
}
int generateID(Client * users, int usersNumb)
{
    
        // Check if there are any users
        if (usersNumb == 0) {
            // If no users, return ID 1
            return 1;
        }
        else {
            // Get the ID of the last user
            int lastUserID = users[usersNumb - 1].userID;
            // Increment the ID by one
            return lastUserID + 1;
        }
    }

string getHash(string inpassword) 
   
{
    SHA1 sha1;
    sha1.update(inpassword);
    return sha1.final();

}

Client* authenticate(int id, string inpassword, Client* users, int usersNumb)
{

    for (int i = 0; i < usersNumb; i++)
    {
        
        //cout << (users + i)->password;
        if ((users + i)->userID == id && getHash(inpassword) == (users + i)->password)
            return users + i;
    }
    return NULL;
}

void bubbleSort(Reservation* reservations, int numbReservations) {
    for (int i = 0; i < numbReservations - 1; ++i) {
        for (int j = 0; j < numbReservations - i - 1; ++j) {
            if (reservations[j].pricePerDay > reservations[j + 1].pricePerDay) {
                // Swap reservations[j] and reservations[j + 1]
                Reservation temp = reservations[j];
                reservations[j] = reservations[j + 1];
                reservations[j + 1] = temp;
            }
        }
    }
}Client *signUp(Client *&users, int &usersNumb)
{
    string lastName, firstName, password, email, phoneNumber;

    // Prompting the user to enter information
    cout << "Please enter the following information for signup:\n";

    do
    {
        cout << "Last name: ";
        cin >> lastName;
        cout << "First name: ";
        cin >> firstName;

        bool hasDigit = false;
        for (char ch : firstName)
        {
            if (isdigit(ch))
            {
                hasDigit = true;
                break;
            }
        }

        if (hasDigit)
        {
            cout << "First name cannot contain digits. Please try again." << endl;
            continue;
        }

        for (char ch : lastName)
        {
            if (isdigit(ch))
            {
                hasDigit = true;
                break;
            }
        }

        if (hasDigit)
        {
            cout << "Last name cannot contain digits. Please try again." << endl;
            continue;
        }

        break; // If no digit found in both first name and last name, exit the loop
    } while (true);

    cout << "Password (must be at least 8 characters long and contain numbers, letters, and special characters): ";
    cin >> password;
    while (!verifyPassword(password))
    {
        cout << "Password (must be at least 8 characters long and contain numbers, letters, and special characters): ";
        cin >> password;
    }

    cout << "Email address: ";
    cin >> email;
    while (!verifyEmail(email))
    {
        cout << "Provide correct email format: ";
        cin >> email;
    }

    cout << "Phone number: ";
    cin >> phoneNumber;
    while (!verifyPhone(phoneNumber))
    {
        cout << "Provide correct phone format: ";
        cin >> phoneNumber;
    }

    Client *nusers = new Client[usersNumb + 1];
    for (int i = 0; i < usersNumb; i++)
    {
        nusers[i] = users[i];
    }

    nusers[usersNumb].userID = generateID(users, usersNumb);
    nusers[usersNumb].firstName = firstName;
    nusers[usersNumb].lastName = lastName;
    nusers[usersNumb].phone = phoneNumber;
    nusers[usersNumb].email = email;
    nusers[usersNumb].password = getHash(password);
    nusers[usersNumb].nbReservation = 0;
    nusers[usersNumb].c = NULL;

    delete[] users;
    users = nusers;

    cout << "Signup successful!\n";
    usersNumb++;
    return &nusers[usersNumb - 1];
}

Car* initializeCars(string filename, int& carsNumb) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return nullptr;
    }

    // Count the number of lines in the file to determine the number of cars
    string line;
    carsNumb = 0;
    while (getline(file, line)) {
        ++carsNumb;
    }

    // Reset file pointer to the beginning of the file
    file.clear();
    file.seekg(0, ios::beg);

    // Allocate memory for cars array
    Car* cars = new Car[carsNumb];
    int carsNum = 0;
    while (getline(file, line)) {
      
    stringstream ss(line);
    string token;
  
    int i = 0; // Initialize index counter
    string  price;
    while (getline(ss, token, ',')) {
        // Process each token (field) here
        switch (i) {
        case 0: cars[carsNum].plateNumber = token; break;
        case 1: cars[carsNum].brand = token; break;
        case 2: cars[carsNum].year = stoi(token); break;
        case 3: cars[carsNum].model = token; break;
        case 4: price = token; 
           
                istringstream(price) >> cars[carsNum].pricePerDay;
          
                 break;
        case 5: cars[carsNum].color = token; break;
        default: break; // Handle additional fields if needed
        }
        i++; // Increment index counter
    }
    // Increment the number of clients after processing each line
    carsNum++;
}
 
    file.close();
    return cars;



}

Client * initializeUsers(string filename , int & usersNumb)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return NULL;
    }

    // Count the number of lines in the file to determine the number of cars
    string line;
    usersNumb = 0;
    while (getline(file, line)) {
        ++usersNumb;
    }

    // Reset file pointer to the beginning of the file
    file.clear();
    file.seekg(0, ios::beg);

    // Allocate memory for cars array
    Client * clients = new Client[usersNumb];
    int clientsNumb = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        int i = 0; // Initialize index counter
        while (getline(ss, token, ',')) {
            // Process each token (field) here
            switch (i) {
            case 0: clients[clientsNumb].userID = stoi(token); break;
            case 1: clients[clientsNumb].firstName = token; break;
            case 2: clients[clientsNumb].lastName = token; break;
            case 3: clients[clientsNumb].password = token; break;
            case 4: clients[clientsNumb].email = token; break;
            case 5: clients[clientsNumb].phone = token; break;
            default: break; // Handle additional fields if needed
            }
            
            i++; // Increment index counter
        }
        clients[clientsNumb].nbReservation = 0;
        // Increment the number of clients after processing each line
        clientsNumb++;
    }

    file.close();
    return clients;


}

void allocateReservation(Client* clients,int clientsNumber, Car* cars,int carsNumber, string filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return;
    }
    /*
    Reservation reservation;
    while (file >> reservation.userID >> reservation.plateNumber >> reservation.startDate >> reservation.endDate) {
        // Find the client and car objects based on the reservation information
        Client* client = nullptr;
        Car* car = nullptr;
        for (int i = 0; i < clientsNumber; ++i) {
            if (clients[i].userID == reservation.userID) {
                client = &clients[i];
                break;
            }
        }
        for (int i = 0; i < carsNumber; ++i) {
            if (cars[i].plateNumber == reservation.plateNumber) {
                car = &cars[i];
                break;
            }
        }
        // If client and car found, update reservation information
        if (client != nullptr && car != nullptr) {
            // Update client's reservation count
            client->nbReservation++;

            // Update client's ordered cars list
            Car* temp = new Car[client->nbReservation]; // Temporary array to hold ordered cars
            for (int i = 0; i < client->nbReservation - 1; ++i) {
                temp[i] = client->c[i]; // Copy existing cars to temporary array
            }
            temp[client->nbReservation - 1] = *car;
            temp[client->nbReservation - 1].d.startDate = reservation.startDate;
            temp[client->nbReservation - 1].d.endDate = reservation.endDate;
            // Add new reservation to temporary array
            //delete[] client->c; // Delete old array
            client->c = temp; // Assign new array to client's ordered cars list

 }
        else {
            cout << "Error: Client or car not found for reservation." << endl;
        }
    }
    */
    Reservation reservation;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        int tokenCount = 0;
        while (getline(iss, token, ',')) {
            switch (tokenCount) {
            case 0: reservation.userID = stoi(token); break;
            case 1: reservation.plateNumber = token; break;
            case 2: reservation.startDate = token; break;
            case 3: reservation.endDate = token; break;
            default: break;
            }
            tokenCount++;
        }

        // Find the client and car objects based on the reservation information
        Client* client = nullptr;
        Car* car = nullptr;
        for (int i = 0; i < clientsNumber; ++i) {
            if (clients[i].userID == reservation.userID) {
                client = &clients[i];
                break;
            }
        }
        for (int i = 0; i < carsNumber; ++i) {
            if (cars[i].plateNumber == reservation.plateNumber) {
                car = &cars[i];
                break;
            }
        }
        // If client and car found, update reservation information
        if (client != nullptr && car != nullptr) {
            // Update client's reservation count
            client->nbReservation++;

            // Update client's ordered cars list
            Car* temp = new Car[client->nbReservation]; // Temporary array to hold ordered cars
            for (int i = 0; i < client->nbReservation - 1; ++i) {
                temp[i] = client->c[i]; // Copy existing cars to temporary array
            }
            temp[client->nbReservation - 1] = *car;
            temp[client->nbReservation - 1].d.startDate = reservation.startDate;
            temp[client->nbReservation - 1].d.endDate = reservation.endDate;
            //delete[] client->c; // Delete old array
            client->c = temp; // Assign new array to client's ordered cars list
        }
        else {
            cout << "Error: Client or car not found for reservation." << endl;
        }
    }
    file.close();

}

void printAllCars(Car* cars, int numbCars) {
    cout << "Available Cars:" << endl;
    // Print table header
    cout << left << setw(15) << "Plate Number" << setw(15) << "Brand" << setw(15) << "Model"
        << setw(10) << "Year" << setw(10) << "Color" << setw(15) << "Price/Day" << endl;

    // Print table rows
    for (int i = 0; i < numbCars; ++i) {
        cout << left << setw(15) << cars[i].plateNumber << setw(15) << cars[i].brand
            << setw(15) << cars[i].model << setw(10) << cars[i].year
            << setw(10) << cars[i].color << setw(15) << cars[i].pricePerDay << endl;
    }
}

bool isDateValid(const string date) {
    // Check if date has valid format (not implemented here, assuming correct format for simplicity)
    // Check if date is greater than or equal to current date
    // Get current time
    time_t currentTime = time(nullptr);
    tm* currentTm = localtime(&currentTime);

    int currentYear = currentTm->tm_year + 1900;
    int currentMonth = currentTm->tm_mon + 1;
    int currentDay = currentTm->tm_mday;

    int year, month, day;
    sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);

    if (year < currentYear) {
        return false;
    }
    else if (year == currentYear && month < currentMonth) {
        return false;
    }
    else if (year == currentYear && month == currentMonth && day < currentDay) {
        return false;
    }

    return true;
}

bool isCarAvailable( string plateNumber,  string rentalDate,int nbOfDays, Client* clients, int numbUsers, int clientID) {
    // Iterate through all clients to check reservations
    
    string endDate = addDaysToDate(rentalDate, nbOfDays);
    for (int i = 0; i < numbUsers; ++i) {
        // Check if the client has any reservations
        
            if (clients[i].nbReservation > 0 && clients[i].userID!= clientID) {
                // Iterate through the reservations of the client
                for (int j = 0; j < clients[i].nbReservation; ++j) {
                    // Check if the reservation matches the selected car
                    if (clients[i].c[j].plateNumber == plateNumber) {
                        // Check if the rental date falls within the reservation period
                        if ((rentalDate >= clients[i].c[j].d.startDate && rentalDate <= clients[i].c[j].d.endDate) || (rentalDate < clients[i].c[j].d.startDate && endDate > clients[i].c[j].d.startDate)) {
                            return false; // Car is not available
                        }
                    }
                }
            }
        
    }
    return true; // Car is available
}

void reserveCar(Client* client, Client* clients, int numbUsers, Car* cars, int numbCars) {
    // Print available cars
    printAllCars(cars, numbCars);

    // Prompt user to choose car by plate number
    string plateNumber;
    cout << "\nEnter the plate number of the car you want to rent: ";
    cin >> plateNumber;

    // Check if the entered plate number exists
    bool carExists = false;
    int carIndex = -1;
    for (int i = 0; i < numbCars; ++i) {
        if (cars[i].plateNumber == plateNumber) {
            carExists = true;
            carIndex = i;
            break;
        }
    }

    if (!carExists) {
        cout << "Car with plate number " << plateNumber << " does not exist. Reservation failed." << endl;
        return;
    }

    // Prompt user for rental date
    string rentalDate;
    cout << "Enter the rental date (YYYY-MM-DD): ";
    cin >> rentalDate;

    // Check if the entered date is valid
    if (!isDateValid(rentalDate)) {
        cout << "Invalid rental date. Date should be greater than or equal to the current date. Reservation failed." << endl;
        return;
    }

    int nbOfDays;
    cout << "Enter the rental duration in(days): ";
    cin >> nbOfDays;

    // Check if the car is available for the rental date
    if (!isCarAvailable(plateNumber, rentalDate, nbOfDays, clients, numbUsers,0)) {
        cout << "The selected car is not available for the specified rental date. Reservation failed." << endl;
        return;
    }

    client->nbReservation++;
    Car* temp = new Car[client->nbReservation];
    if (client->c != NULL)
    {
        for (int i = 0; i < client->nbReservation - 1; ++i) {
            temp[i] = client->c[i];
        }
        temp[client->nbReservation - 1] = cars[carIndex];
        temp[client->nbReservation - 1].d.startDate = rentalDate;
        temp[client->nbReservation - 1].d.endDate = addDaysToDate(rentalDate, nbOfDays);
        //delete[] client->c;
        client->c = temp;
    }
    else
    {
        client->c= new Car[client->nbReservation];
        client->c[client->nbReservation - 1] = cars[carIndex];
        client->c[client->nbReservation - 1].d.startDate = rentalDate;
        client->c[client->nbReservation - 1].d.endDate = addDaysToDate(rentalDate, nbOfDays);

    }

    cout << "Reservation successful!" << endl;
  
}
void changeReservationDate(Client* client, Client* clients, int numbUsers) {
    // Print client's current reservations
    cout << "Current Reservations:" << endl;
    if (client->nbReservation == 0) {
        cout << "No reservations found for this client." << endl;
        return;
    }
    // Print table header
    cout << left << setw(15) << "Plate Number" << setw(15) << "Start Date" << setw(15) << "End Date" << endl;

    // Print table rows
    for (int i = 0; i < client->nbReservation; ++i) {
        cout << left << setw(15) << client->c[i].plateNumber << setw(15) << client->c[i].d.startDate
            << setw(15) << client->c[i].d.endDate << endl;
    }

    // Prompt user to enter plate number of the reserved car
    string plateNumber;
    cout << "\nEnter the plate number of the reserved car: ";
    cin >> plateNumber;

    // Find the reserved car
    int reservedCarIndex = -1;
    for (int i = 0; i < client->nbReservation; ++i) {
        if (client->c[i].plateNumber == plateNumber) {
            reservedCarIndex = i;
            break;
        }
    }

    // Check if the reserved car exists
    if (reservedCarIndex == -1) {
        cout << "Car with plate number " << plateNumber << " is not found in your reservations." << endl;
        return;
    }

    // Prompt user to enter new reservation date
    string newReservationDate;
    cout << "Enter the new reservation start date (YYYY-MM-DD): ";
    cin >> newReservationDate;

    // Check if the entered date is valid
    if (!isDateValid(newReservationDate)) {
        cout << "Invalid reservation start date. Date should be greater than the current date." << endl;
        return;
    }

    // Prompt user to enter number of days for the rental
    int nbOfDays;
    cout << "Enter the number of days for the rental: ";
    cin >> nbOfDays;

    // Check if the car is available for the new reservation date
    if (!isCarAvailable(plateNumber, newReservationDate, nbOfDays, clients, numbUsers, client->userID)) {
        cout << "The selected car is not available for the specified reservation period." << endl;
        return;
    }

    // Update reservation details
    client->c[reservedCarIndex].d.startDate = newReservationDate;
    client->c[reservedCarIndex].d.endDate = addDaysToDate(newReservationDate, nbOfDays);

    cout << "Reservation date changed successfully." << endl;
}
void cancelReservation(Client* client)
{
   
    cout << "Current Reservations:" << endl;
    if (client->nbReservation == 0) {
        cout << "No reservations found for this client." << endl;
        return;
    }
    // Print table header
    cout << left << setw(15) << "Plate Number" << setw(15) << "Start Date" << setw(15) << "End Date" << endl;

    // Print table rows
    for (int i = 0; i < client->nbReservation; ++i) {
        cout << left << setw(15) << client->c[i].plateNumber << setw(15) << client->c[i].d.startDate
            << setw(15) << client->c[i].d.endDate << endl;
    }
    bool found = false;
    string plateNumber;
    cout << "\nEnter the plate number of the reserved car: ";
    cin >> plateNumber;
    for (int i = 0; i < client->nbReservation; ++i) {
        if (client->c[i].plateNumber == plateNumber) {
            // Shift the remaining reservations to the left
            for (int j = i; j < client->nbReservation - 1; ++j) {
                client->c[j] = client->c[j + 1];
            }
            // Decrement the number of reservations
            client->nbReservation--;
            found = true;
            break;
        }
    }

    if (found) {
        cout << "Reservation for car " << plateNumber << " canceled successfully.\n" << endl;
    }
    else {
        cout << "Error: Reservation not found for car " << plateNumber << "." << endl;
    }

}void saveData(Car * cars , int numbCars , Client * clients , int clientsNumb ) {
    // Count total number of reservations
    int totalReservations = 0;
    for (int i = 0; i < clientsNumb; ++i) {
        totalReservations += clients[i].nbReservation;
    }

    // Create the reservationCars array with the exact size needed
    Reservation* reservationCars = new Reservation[totalReservations];

    // Fill reservationCars array with data from clients
    int index = 0;
    for (int i = 0; i < clientsNumb; ++i) {
        for (int j = 0; j < clients[i].nbReservation; ++j) {
            reservationCars[index].userID = clients[i].userID;
            reservationCars[index].plateNumber = clients[i].c[j].plateNumber;
            reservationCars[index].pricePerDay = clients[i].c[j].pricePerDay;
            reservationCars[index].startDate = clients[i].c[j].d.startDate;
            reservationCars[index].endDate = clients[i].c[j].d.endDate;
            index++;
        }
    }
    
    // Sort reservationCars array by pricePerDay
    bubbleSort(reservationCars, totalReservations);

    // Update users.csv
    ofstream usersFile("users.csv");
    if (!usersFile.is_open()) {
        cout << "Error: Unable to open users.csv for writing." << endl;
        delete[] reservationCars; // Clean up dynamically allocated memory
        return;
    }

    for (int i = 0; i < clientsNumb; ++i) {
        usersFile << clients[i].userID << "," << clients[i].firstName << "," << clients[i].lastName << ","
            << clients[i].password << "," << clients[i].email << "," << clients[i].phone << endl;
    }

    usersFile.close();

    // Update rental cars.csv
    ofstream rentalCarsFile("rental cars.csv");
    if (!rentalCarsFile.is_open()) {
        cout << "Error: Unable to open rental cars.csv for writing." << endl;
        delete[] reservationCars; // Clean up dynamically allocated memory
        return;
    }
     
    for (int i = 0; i < totalReservations; ++i) {
        rentalCarsFile << reservationCars[i].userID << "," << reservationCars[i].plateNumber << "," << reservationCars[i].startDate << ","
            << reservationCars[i].endDate << endl;
    }

    rentalCarsFile.close();

    // Update cars.csv
    ofstream carsFile("cars.csv");
    if (!carsFile.is_open()) {
        cout << "Error: Unable to open cars.csv for writing." << endl;
        delete[] reservationCars; // Clean up dynamically allocated memory
        return;
    }

    for (int i = 0; i < numbCars; ++i) {
        carsFile << cars[i].plateNumber << "," << cars[i].brand << "," << cars[i].year << "," << cars[i].model << ","
            << to_string(cars[i].pricePerDay)<< "," << cars[i].color << endl;
    }

    carsFile.close();

    // Convert rental cars data to PDF
 convertToPDF("rental cars.csv", "rental cars.pdf");

    // Clean up dynamically allocated memory
    delete[] reservationCars;
}

void addCar(Car * & cars , int & carsNumb) {
    Car newCar;
    cout << "Enter plate number: ";
    cin >> newCar.plateNumber;

    // Check if the entered plate number already exists
    for (int i = 0; i < carsNumb; ++i) {
        if (cars[i].plateNumber == newCar.plateNumber) {
            cout << "Plate number already exists. Please enter a unique plate number.\n";
            return; // Exit the function if plate number already exists
        }
    }

    cout << "Enter brand: ";
    cin >> newCar.brand;
    cout << "Enter model: ";
    cin >> newCar.model;
    cout << "Enter year: ";
    cin >> newCar.year;
    cout << "Enter color: ";
    cin >> newCar.color;
    cout << "Enter price per day: ";
    cin >> newCar.pricePerDay;

    // Increase the size of the cars array by 1
    Car* temp = new Car[carsNumb + 1];
    // Copy existing cars to the new array
    for (int i = 0; i < carsNumb; ++i) {
        temp[i] = cars[i];
    }
    // Add the new car at the end
    temp[carsNumb] = newCar;
    // Delete the old array
    delete[] cars;
    // Point cars to the new array
    cars = temp;
    // Increment the number of cars
    carsNumb++;
}

void deleteCar(Car * & cars, int & carsNumb, Client* clients, int numbUser) {
    printAllCars(cars, carsNumb);
    string plateNumber;
    cout << "Enter plate number of car to update: ";
    cin >> plateNumber;
    int index = -1;
    for (int i = 0; i < carsNumb; ++i) {
        if (cars[i].plateNumber == plateNumber) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return; // Car not found
    }

    // Remove the car from reservations of all clients
    for (int j = 0; j < numbUser; ++j) {
        for (int k = 0; k < clients[j].nbReservation; ++k) {
            if (clients[j].c[k].plateNumber == plateNumber) {
                // Shift remaining reserved cars to fill the gap
                for (int m = k; m < clients[j].nbReservation - 1; ++m) {
                    clients[j].c[m] = clients[j].c[m + 1];
                }
                // Decrement the number of reserved cars for the client
                clients[j].nbReservation--;
                break;
            }
        }
    }

    // Move all cars after the deleted car one position forward
    for (int i = index; i < carsNumb - 1; ++i) {
        cars[i] = cars[i + 1];
    }
    // Decrease the size of the cars array by 1
    Car* temp = new Car[carsNumb - 1];
    // Copy existing cars to the new array
    for (int i = 0; i < carsNumb - 1; ++i) {
        temp[i] = cars[i];
    }
    // Delete the old array
    delete[] cars;
    // Point cars to the new array
    cars = temp;
    // Decrement the number of cars
    carsNumb--;
}

void updateCar(Car* cars, int carsNumb, Client * clients , int numbUser) {

  
    printAllCars(cars, carsNumb);
    string plateNumber;
    cout << "Enter plate number of car to update: ";
    cin >> plateNumber;
    // Check if the entered plate number already exists
    bool plateNumberExists = false;
    for (int i = 0; i < carsNumb; ++i) {
        if (cars[i].plateNumber == plateNumber) {
            plateNumberExists = true;
            break;
        }
    }

    if (!plateNumberExists) {
        cout << "Plate number does not exist.\n";
        return;
    }

    for (int i = 0; i < carsNumb; ++i) {
        if (cars[i].plateNumber == plateNumber) {
            cout << "Enter new plate number: ";
            cin >> cars[i].plateNumber;

            // Check if the new plate number is unique
            bool newPlateNumberUnique = true;
            for (int j = 0; j < carsNumb; ++j) {
                if (i != j && cars[j].plateNumber == cars[i].plateNumber) {
                    newPlateNumberUnique = false;
                    break;
                }
            }
            if (!newPlateNumberUnique) {
                cout << "Plate number already exists. Please enter a unique plate number.\n";
                return;
            }
            cout << "Enter new brand: ";
            cin >> cars[i].brand;
            cout << "Enter new model: ";
            cin >> cars[i].model;
            cout << "Enter new year: ";
            cin >> cars[i].year;
            cout << "Enter new color: ";
            cin >> cars[i].color;
            cout << "Enter new price per day: ";
            cin >> cars[i].pricePerDay;
            // Update reserved cars for each client
            for (int j = 0; j < numbUser; ++j) {
                for (int k = 0; k < clients[j].nbReservation; ++k) {
                    if (clients[j].c[k].plateNumber == plateNumber) {
                        string tempstartDate = clients[j].c[k].d.startDate;
                        string tempendDate = clients[j].c[k].d.endDate;
                        clients[j].c[k] = cars[i];
                        clients[j].c[k].d.startDate = tempstartDate;
                        clients[j].c[k].d.endDate = tempendDate;
                        // Update car details
                        break;
                    }
                }
            }

            cout << "Car is updated successfully\n";
            break;
            break;
        }
    }

}
void printClientReservations(Client* client) {
    if (client == nullptr || client->c == nullptr || client->nbReservation == 0) {
        cout << "No reservations found for this client." << endl;
        return;
    }

    // Print table header
    cout << left << setw(15) << "Plate Number" << setw(15) << "Brand" << setw(15) << "Model"
        << setw(10) << "Year" << setw(10) << "Color" << setw(15) << "Price/Day"
        << setw(15) << "Start Date" << setw(15) << "End Date" << endl;

    // Print table rows
    for (int i = 0; i < client->nbReservation; ++i) {
        cout << left << setw(15) << client->c[i].plateNumber << setw(15) << client->c[i].brand
            << setw(15) << client->c[i].model << setw(10) << client->c[i].year
            << setw(10) << client->c[i].color << setw(15) << client->c[i].pricePerDay
            << setw(15) << client->c[i].d.startDate << setw(15) << client->c[i].d.endDate << endl;
    }
}

int main()
{
    int choice=0;
    bool isAdmin = false;
    Client* users = nullptr;
    int usersNumb = 0;
    Car* cars = nullptr;
    int carsNumb = 0;
    Client* currentUser=NULL;
    // Initialize users and cars
    users = initializeUsers("users.csv", usersNumb);
    cars = initializeCars("cars.csv", carsNumb);
    allocateReservation(users, usersNumb, cars, carsNumb, "rental cars.csv");
    // Authentication
    cout << "1. Login\n2. Signup\nEnter your choice: ";
    cin >> choice;

    if (choice == 1) {
        int id;
        string password;
        cout << "Enter your ID: ";
        cin >> id;

        cout << "Enter your password: ";
        cin >> password;
        currentUser = authenticate(id, password, users, usersNumb);
        if (currentUser != nullptr) {
            cout << "Login successful!" << endl;
            if (currentUser->userID == 1) {
                isAdmin = true;
            }
        }
        else {
            cout << "Invalid credentials. Exiting..." << endl;
            return 0;
        }
    }
    else if (choice == 2) {
        currentUser=  signUp(users, usersNumb);
    }
    else {
        cout << "Invalid choice. Exiting..." << endl;
        return 0;
    }

    // Admin or normal user operations
    if (isAdmin || currentUser->userID == 1) {
        string cont;
        do {
        // Admin operations
        cout << "Admin options:\n1. Add Car\n2. Delete Car\n3. Update Car\n0. Exit\nEnter your choice: ";
        cin >> choice;
      
            string plateNumber = "";
            switch (choice) {
            case 1:
                addCar(cars, carsNumb);
                break;
            case 2:


                deleteCar(cars, carsNumb, users, usersNumb);
                break;
            case 3:

                updateCar(cars, carsNumb, users, usersNumb);
                break;
            default:
                cout << "Exiting..." << endl;
                saveData(cars, carsNumb, users, usersNumb);
                return 0;

                
            }
            cout << "whould you like to continue (yes/no)\n";
            cin >> cont;

        } while (cont == "yes");
    }
    else {

        string cont;
        do {
            // Normal user operations
            string plateNumber;
            cout << "Normal user options:\n1. Rent Car\n2. Cancel Reservation\n3. Update Reservation\n0. Exit\nEnter your choice: ";
            cin >> choice;
            switch (choice) {
            case 1:
                reserveCar(currentUser, users, usersNumb, cars, carsNumb);

                break;
            case 2:
                //Cancel reservation
                cancelReservation(currentUser);
                break;
            case 3:
                // Update reservation
                changeReservationDate(currentUser, users, usersNumb);
                break;
            default:
                cout << "Exiting..." << endl;
                saveData(cars, carsNumb, users, usersNumb);
                return 0;
            }
            cout << "whould you like to continue (yes/no)\n";
            cin >> cont;

        } while (cont == "yes");
    }
    saveData(cars, carsNumb, users, usersNumb);

    // Clean up allocated memory
    delete[] users;
    delete[] cars;

    return 0;
   
}
