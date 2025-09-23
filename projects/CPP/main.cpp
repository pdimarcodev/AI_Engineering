
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

class Person{

    protected: string idCard, firstName, lastName, email;

    public:
        void setIdCard(const string& id) { idCard = id; }
        string getIdCard() const { return idCard; }

        void setFirstName(const string& fname) { firstName = fname; }
        string getFirstName() const { return firstName; }

        void setLastName(const string& lname) { lastName = lname; }
        string getLastName() const { return lastName; }

        void setEmail(const string& em) { email = em; }
        string getEmail() const { return email; }

};

class Company{
  private: string name;

  public:
    void setName(const string& n) { name = n; }
    string getName() const { return name; }
};

class Interaction {
    protected:
        string description;
        string type;

    public:
        Interaction(const string& desc, const string& t) : description(desc), type(t) {}
        virtual ~Interaction() = default;

        void setDescription(const string& desc) { description = desc; }
        string getDescription() const { return description; }

        void setType(const string& t) { type = t; }
        string getType() const { return type; }

        virtual string toString() const = 0;
};

class Appointment : public Interaction {
    private:
        string salesPerson;
        string date;
        string hour;

    public:
        Appointment(const string& desc, const string& salesperson, const string& dt, const string& hr)
            : Interaction(desc, "Appointment"), salesPerson(salesperson), date(dt), hour(hr) {}

        void setSalesPerson(const string& sp) { salesPerson = sp; }
        string getSalesPerson() const { return salesPerson; }

        void setDate(const string& dt) { date = dt; }
        string getDate() const { return date; }

        void setHour(const string& hr) { hour = hr; }
        string getHour() const { return hour; }

        string toString() const override {
            return "Appointment - " + description + " (Sales: " + salesPerson + ", Date: " + date + ", Hour: " + hour + ")";
        }
};

class Contract : public Interaction {
    private:
        double value;
        string status;

    public:
        Contract(const string& desc, double val, const string& stat)
            : Interaction(desc, "Contract"), value(val), status(stat) {}

        void setValue(double val) { value = val; }
        double getValue() const { return value; }

        void setStatus(const string& stat) { status = stat; }
        string getStatus() const { return status; }

        string toString() const override {
            stringstream ss;
            ss << fixed << setprecision(2) << value;
            return "Contract - " + description + " (Value: $" + ss.str() + ", Status: " + status + ")";
        }
};

class Client : public Person{

    private:
      int policyNumber;
      Company company;

    public:
      void setPolicyNumber(int pnum) { policyNumber = pnum; }
      int getPolicyNumber() const { return policyNumber; }

      void setCompany(const Company& comp) { company = comp; }
      Company getCompany() const { return company; }

};

class ClientManager {
    private:
        vector<Client> clients;
        map<string, vector<Interaction*>> clientInteractions;

    public:
        ~ClientManager() {
            for (auto& pair : clientInteractions) {
                for (auto* interaction : pair.second) {
                    delete interaction;
                }
            }
        }

        void addClient(const Client& client) {
            clients.push_back(client);
        }

        void displayAllClients() const {
            if (clients.empty()) {
                cout << "No clients found.\n";
                return;
            }

            cout << "\n=== ALL CLIENTS ===\n";
            for (size_t i = 0; i < clients.size(); i++) {
                cout << "[" << i + 1 << "] ";
                cout << "ID: " << clients[i].getIdCard() << " | ";
                cout << "Name: " << clients[i].getFirstName() << " " << clients[i].getLastName() << " | ";
                cout << "Email: " << clients[i].getEmail() << " | ";
                cout << "Policy: " << clients[i].getPolicyNumber() << " | ";
                cout << "Company: " << clients[i].getCompany().getName() << "\n";
            }
        }

        bool editClient(int index) {
            if (index < 0 || index >= static_cast<int>(clients.size())) {
                cout << "Invalid client index.\n";
                return false;
            }

            Client& client = clients[index];
            string input;
            int choice;

            cout << "\nEditing client: " << client.getFirstName() << " " << client.getLastName() << "\n";
            cout << "1. Edit ID Card\n2. Edit First Name\n3. Edit Last Name\n";
            cout << "4. Edit Email\n5. Edit Policy Number\n6. Edit Company Name\n";
            cout << "Choose field to edit: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: {
                    cout << "Enter new ID Card: ";
                    getline(cin, input);
                    client.setIdCard(input);
                    break;
                }
                case 2: {
                    cout << "Enter new First Name: ";
                    getline(cin, input);
                    client.setFirstName(input);
                    break;
                }
                case 3: {
                    cout << "Enter new Last Name: ";
                    getline(cin, input);
                    client.setLastName(input);
                    break;
                }
                case 4: {
                    cout << "Enter new Email: ";
                    getline(cin, input);
                    client.setEmail(input);
                    break;
                }
                case 5: {
                    cout << "Enter new Policy Number: ";
                    cin >> choice;
                    client.setPolicyNumber(choice);
                    break;
                }
                case 6: {
                    cout << "Enter new Company Name: ";
                    getline(cin, input);
                    Company newCompany;
                    newCompany.setName(input);
                    client.setCompany(newCompany);
                    break;
                }
                default:
                    cout << "Invalid choice.\n";
                    return false;
            }
            cout << "Client updated successfully!\n";
            return true;
        }

        bool deleteClient(int index) {
            if (index < 0 || index >= static_cast<int>(clients.size())) {
                cout << "Invalid client index.\n";
                return false;
            }

            string clientId = clients[index].getIdCard();
            clients.erase(clients.begin() + index);

            auto it = clientInteractions.find(clientId);
            if (it != clientInteractions.end()) {
                for (auto* interaction : it->second) {
                    delete interaction;
                }
                clientInteractions.erase(it);
            }

            cout << "Client deleted successfully!\n";
            return true;
        }

        vector<int> searchClients(const string& searchTerm) const {
            vector<int> results;
            string lowerSearchTerm = searchTerm;
            transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);

            for (size_t i = 0; i < clients.size(); i++) {
                string firstName = clients[i].getFirstName();
                string lastName = clients[i].getLastName();
                transform(firstName.begin(), firstName.end(), firstName.begin(), ::tolower);
                transform(lastName.begin(), lastName.end(), lastName.begin(), ::tolower);

                if (firstName.find(lowerSearchTerm) != string::npos ||
                    lastName.find(lowerSearchTerm) != string::npos) {
                    results.push_back(i);
                }
            }
            return results;
        }

        void addInteraction(const string& clientId, Interaction* interaction) {
            clientInteractions[clientId].push_back(interaction);
        }

        void displayClientInteractions(const string& clientId) const {
            auto it = clientInteractions.find(clientId);
            if (it == clientInteractions.end() || it->second.empty()) {
                cout << "No interactions found for this client.\n";
                return;
            }

            cout << "\n=== INTERACTIONS ===\n";
            for (size_t i = 0; i < it->second.size(); i++) {
                cout << "[" << i + 1 << "] " << it->second[i]->toString() << "\n";
            }
        }

        const vector<Client>& getClients() const { return clients; }
        void setClients(const vector<Client>& newClients) { clients = newClients; }

        const map<string, vector<Interaction*>>& getInteractions() const { return clientInteractions; }
};

class FileManager {
    public:
        static bool saveToCSV(const string& filename, const ClientManager& manager) {
            ofstream file(filename);
            if (!file.is_open()) {
                cout << "Error: Could not open file for writing.\n";
                return false;
            }

            file << "ID_Card,First_Name,Last_Name,Email,Policy_Number,Company_Name,Interaction_Type,Description,Sales_Person,Date,Hour,Value,Status\n";

            const auto& clients = manager.getClients();
            const auto& interactions = manager.getInteractions();

            for (const auto& client : clients) {
                string clientId = client.getIdCard();
                auto it = interactions.find(clientId);
                if (it != interactions.end() && !it->second.empty()) {
                    for (const auto* interaction : it->second) {
                        file << client.getIdCard() << ","
                             << client.getFirstName() << ","
                             << client.getLastName() << ","
                             << client.getEmail() << ","
                             << client.getPolicyNumber() << ","
                             << client.getCompany().getName() << ","
                             << interaction->getType() << ","
                             << interaction->getDescription();

                        if (interaction->getType() == "Appointment") {
                            const Appointment* apt = dynamic_cast<const Appointment*>(interaction);
                            file << "," << apt->getSalesPerson() << "," << apt->getDate() << "," << apt->getHour() << ",,";
                        } else if (interaction->getType() == "Contract") {
                            const Contract* contract = dynamic_cast<const Contract*>(interaction);
                            stringstream ss;
                            ss << fixed << setprecision(2) << contract->getValue();
                            file << ",,," << ss.str() << "," << contract->getStatus();
                        }
                        file << "\n";
                    }
                } else {
                    file << client.getIdCard() << ","
                         << client.getFirstName() << ","
                         << client.getLastName() << ","
                         << client.getEmail() << ","
                         << client.getPolicyNumber() << ","
                         << client.getCompany().getName() << ","
                         << ",,,,,,\n";
                }
            }

            file.close();
            cout << "Data saved to " << filename << " successfully!\n";
            return true;
        }


        static bool loadFromCSV(const string& filename, ClientManager& manager) {
            ifstream file(filename);
            if (!file.is_open()) {
                cout << "Warning: Could not open file " << filename << ". Starting with empty database.\n";
                return false;
            }

            string line;
            getline(file, line);

            vector<Client> clients;
            map<string, bool> clientExists;

            while (getline(file, line)) {
                if (line.empty()) continue;

                stringstream ss(line);
                string idCard, firstName, lastName, email, policyStr, companyName;
                string interactionType, description, salesPerson, date, hour, valueStr, status;

                getline(ss, idCard, ',');
                getline(ss, firstName, ',');
                getline(ss, lastName, ',');
                getline(ss, email, ',');
                getline(ss, policyStr, ',');
                getline(ss, companyName, ',');
                getline(ss, interactionType, ',');
                getline(ss, description, ',');
                getline(ss, salesPerson, ',');
                getline(ss, date, ',');
                getline(ss, hour, ',');
                getline(ss, valueStr, ',');
                getline(ss, status, ',');

                try {
                    if (clientExists.find(idCard) == clientExists.end()) {
                        int policyNumber = stoi(policyStr);

                        Client client;
                        client.setIdCard(idCard);
                        client.setFirstName(firstName);
                        client.setLastName(lastName);
                        client.setEmail(email);
                        client.setPolicyNumber(policyNumber);

                        Company company;
                        company.setName(companyName);
                        client.setCompany(company);

                        clients.push_back(client);
                        clientExists[idCard] = true;
                    }

                    if (!interactionType.empty()) {
                        if (interactionType == "Appointment") {
                            manager.addInteraction(idCard, new Appointment(description, salesPerson, date, hour));
                        } else if (interactionType == "Contract") {
                            double value = 0.0;
                            if (!valueStr.empty()) {
                                try {
                                    value = stod(valueStr);
                                } catch (const exception& e) {
                                    cout << "Warning: Invalid contract value '" << valueStr << "', using 0.0\n";
                                }
                            }
                            manager.addInteraction(idCard, new Contract(description, value, status));
                        }
                    }
                } catch (const exception& e) {
                    cout << "Error loading data: " << e.what() << endl;
                }
            }

            manager.setClients(clients);
            file.close();
            cout << "Data loaded from " << filename << " successfully! (" << clients.size() << " clients)\n";
            return true;
        }

};

class UserInterface {
    private:
        ClientManager& manager;
        const string filename = "crm_data.csv";

    public:
        UserInterface(ClientManager& mgr) : manager(mgr) {}

        void displayMainMenu() {
            cout << "\n=== CRM SYSTEM ===\n";
            cout << "1. Add Client\n";
            cout << "2. View All Clients\n";
            cout << "3. Edit Client\n";
            cout << "4. Delete Client\n";
            cout << "5. Search Client\n";
            cout << "6. Manage Interactions\n";
            cout << "7. Save Data\n";
            cout << "8. Load Data\n";
            cout << "9. Exit\n";
            cout << "Choose option: ";
        }

        void addClientFlow() {
            cout << "\n=== ADD CLIENT ===\n";
            string idCard, firstName, lastName, email, companyName;
            int policyNumber;

            cout << "Enter ID Card: ";
            cin.ignore();
            getline(cin, idCard);

            cout << "Enter First Name: ";
            getline(cin, firstName);

            cout << "Enter Last Name: ";
            getline(cin, lastName);

            cout << "Enter Email: ";
            getline(cin, email);

            cout << "Enter Policy Number: ";
            cin >> policyNumber;

            cout << "Enter Company Name: ";
            cin.ignore();
            getline(cin, companyName);

            Client client;
            client.setIdCard(idCard);
            client.setFirstName(firstName);
            client.setLastName(lastName);
            client.setEmail(email);
            client.setPolicyNumber(policyNumber);

            Company company;
            company.setName(companyName);
            client.setCompany(company);

            manager.addClient(client);
            cout << "Client added successfully!\n";
        }

        void editClientFlow() {
            if (manager.getClients().empty()) {
                cout << "No clients found.\n";
                return;
            }

            cout << "\nEnter client's first or last name to search: ";
            string searchTerm;
            cin.ignore();
            getline(cin, searchTerm);

            vector<int> results = manager.searchClients(searchTerm);
            if (results.empty()) {
                cout << "No clients found with that name.\n";
                return;
            }

            if (results.size() == 1) {
                manager.editClient(results[0]);
            } else {
                cout << "\nMultiple clients found:\n";
                const auto& clients = manager.getClients();
                for (size_t i = 0; i < results.size(); i++) {
                    int index = results[i];
                    cout << "[" << i + 1 << "] ";
                    cout << "Name: " << clients[index].getFirstName() << " " << clients[index].getLastName();
                    cout << " | Email: " << clients[index].getEmail() << "\n";
                }

                cout << "\nEnter the number of the client to edit: ";
                int choice;
                cin >> choice;

                if (choice >= 1 && choice <= static_cast<int>(results.size())) {
                    manager.editClient(results[choice - 1]);
                } else {
                    cout << "Invalid choice.\n";
                }
            }
        }

        void deleteClientFlow() {
            manager.displayAllClients();
            if (manager.getClients().empty()) return;

            cout << "\nEnter client number to delete: ";
            int index;
            cin >> index;

            cout << "Are you sure? (y/n): ";
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                manager.deleteClient(index - 1);
            }
        }

        void searchClientFlow() {
            cout << "\nEnter search term (first/last name): ";
            string searchTerm;
            cin.ignore();
            getline(cin, searchTerm);

            vector<int> results = manager.searchClients(searchTerm);
            if (results.empty()) {
                cout << "No clients found.\n";
                return;
            }

            cout << "\n=== SEARCH RESULTS ===\n";
            const auto& clients = manager.getClients();
            for (int index : results) {
                cout << "[" << index + 1 << "] ";
                cout << "Name: " << clients[index].getFirstName() << " " << clients[index].getLastName();
                cout << " | Email: " << clients[index].getEmail() << "\n";
            }
        }

        void manageInteractionsFlow() {
            manager.displayAllClients();
            if (manager.getClients().empty()) return;

            cout << "\nEnter client number: ";
            int index;
            cin >> index;

            if (index < 1 || index > static_cast<int>(manager.getClients().size())) {
                cout << "Invalid client number.\n";
                return;
            }

            const Client& client = manager.getClients()[index - 1];
            string clientId = client.getIdCard();

            cout << "\n1. Add Appointment\n2. Add Contract\n3. View Interactions\nChoose: ";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1: {
                    cout << "Enter appointment description: ";
                    string desc, salesperson, date, hour;
                    cin.ignore();
                    getline(cin, desc);
                    cout << "Enter salesperson name: ";
                    getline(cin, salesperson);
                    cout << "Enter appointment date (YYYY-MM-DD): ";
                    getline(cin, date);
                    cout << "Enter appointment hour (HH:MM): ";
                    getline(cin, hour);
                    manager.addInteraction(clientId, new Appointment(desc, salesperson, date, hour));
                    cout << "Appointment added!\n";
                    break;
                }
                case 2: {
                    cout << "Enter contract description: ";
                    string desc, status;
                    double value;
                    cin.ignore();
                    getline(cin, desc);
                    cout << "Enter contract value: ";
                    cin >> value;
                    cout << "Enter contract status: ";
                    cin.ignore();
                    getline(cin, status);
                    manager.addInteraction(clientId, new Contract(desc, value, status));
                    cout << "Contract added!\n";
                    break;
                }
                case 3:
                    manager.displayClientInteractions(clientId);
                    break;
                default:
                    cout << "Invalid choice.\n";
            }
        }

        void run() {
            FileManager::loadFromCSV(filename, manager);

            int choice;
            do {
                displayMainMenu();
                cin >> choice;

                switch (choice) {
                    case 1: addClientFlow(); break;
                    case 2: manager.displayAllClients(); break;
                    case 3: editClientFlow(); break;
                    case 4: deleteClientFlow(); break;
                    case 5: searchClientFlow(); break;
                    case 6: manageInteractionsFlow(); break;
                    case 7: FileManager::saveToCSV(filename, manager); break;
                    case 8: FileManager::loadFromCSV(filename, manager); break;
                    case 9:
                        FileManager::saveToCSV(filename, manager);
                        cout << "Shutting down!\n";
                        break;
                    default: cout << "Invalid choice.\n";
                }
            } while (choice != 9);
        }
};

int main() {
    cout << "InsuraPro CRM\n";
    cout << "=============\n";

    ClientManager manager;
    UserInterface ui(manager);

    ui.run();

    return 0;
}
