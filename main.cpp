#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Property {
protected:
    string address;
    double price;

public:
    Property(const string& addr = "", double pr = 0) : address(addr), price(pr) {}

    virtual void displayDetails() = 0;

    friend ifstream& operator>>(ifstream& inputFile, Property& property);
    friend ostream& operator<<(ostream& outputStream, Property& property);
};

ifstream& operator>>(ifstream& inputFile, Property& property) {
    inputFile >> property.address >> property.price;
    return inputFile;
}

ostream& operator<<(ostream& outputStream, Property& property) {
    outputStream << "Address: " << property.address << endl;
    outputStream << "Price: $" << property.price << endl;
    return outputStream;
}

class ResidentialProperty : public Property {
private:
    int bedrooms;
    int bathrooms;

public:
    ResidentialProperty(const string& addr = "", double pr = 0, int beds = 0, int baths = 0)
        : Property(addr, pr), bedrooms(beds), bathrooms(baths) {}

    void displayDetails() override {
        cout << "Residential Property Details:" << endl;
        cout << *this;
        cout << "Bedrooms: " << bedrooms << endl;
        cout << "Bathrooms: " << bathrooms << endl;
        cout << endl;
    }

    friend ifstream& operator>>(ifstream& inputFile, ResidentialProperty& resProperty);
};

ifstream& operator>>(ifstream& inputFile, ResidentialProperty& resProperty) {
    inputFile >> static_cast<Property&>(resProperty);
    inputFile >> resProperty.bedrooms >> resProperty.bathrooms;
    return inputFile;
}

class CommercialProperty : public Property {
private:
    int floors;
    int rooms;

public:
    CommercialProperty(const string& addr = "", double pr = 0, int flrs = 0, int rms = 0)
        : Property(addr, pr), floors(flrs), rooms(rms) {}

    void displayDetails() override {
        cout << "Commercial Property Details:" << endl;
        cout << *this;
        cout << "Floors: " << floors << endl;
        cout << "Rooms: " << rooms << endl;
        cout << endl;
    }

    friend ifstream& operator>>(ifstream& inputFile, CommercialProperty& comProperty);
};

ifstream& operator>>(ifstream& inputFile, CommercialProperty& comProperty) {
    inputFile >> static_cast<Property&>(comProperty);
    inputFile >> comProperty.floors >> comProperty.rooms;
    return inputFile;
}

class RealEstateAgency {
private:
    vector<Property*> properties;

public:
    void readPropertiesFromFile(const string& fileName) {
        ifstream inputFile(fileName);
        if (!inputFile) {
            cout << "Error opening file." << endl;
            return;
        }

        char propertyType;
        while (inputFile >> propertyType) {
            if (propertyType == 'R') {
                ResidentialProperty* resProperty = new ResidentialProperty();
                inputFile >> *resProperty;
                properties.push_back(resProperty);
            } else if (propertyType == 'C') {
                CommercialProperty* comProperty = new CommercialProperty();
                inputFile >> *comProperty;
                properties.push_back(comProperty);
            }
        }

        inputFile.close();
    }

    void displayAllProperties() {
        cout << "----- Real Estate Agency -----" << endl;
        for (Property* property : properties) {
            property->displayDetails();
        }
    }

    ~RealEstateAgency() {
        for (Property* property : properties) {
            delete property;
        }
    }
};

int main() {
    RealEstateAgency agency;
    agency.readPropertiesFromFile("properties.txt");
    agency.displayAllProperties();

    return 0;
}
