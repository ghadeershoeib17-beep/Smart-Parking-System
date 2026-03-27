#include <iostream>
#include <stack>
#include <queue>
#include <string>
using namespace std;

struct HistoryNode {
    int carID;
    string model;
    string action;
    HistoryNode* next;

    HistoryNode(int id, string m, string a) {
        carID = id;
        model = m;
        action = a;
        next = NULL;
    }
};

class HistoryList {
private:
    HistoryNode* head;

public:
    HistoryList() { head = NULL; }

    void addHistory(int id, string model, string action) {
        HistoryNode* newNode = new HistoryNode(id, model, action);
        if (head == NULL) {
            head = newNode;
            return;
        }
        HistoryNode* current = head;
        while (current->next != NULL)
            current = current->next;
        current->next = newNode;
    }

    void displayHistory() {
        cout << "\n--- Parking History (Oldest -> Newest) ---\n";
        if (!head) {
            cout << "No history available.\n";
            return;
        }
        HistoryNode* current = head;
        while (current) {
            cout << "Car ID: " << current->carID
                 << " | Model: " << current->model
                 << " | Action: " << current->action << endl;
            current = current->next;
        }
        cout << "-------------------------------------------\n";
    }
};

struct Car {
    int id;
    string model;
    Car(int i = 0, string m = "") {
        id = i;
        model = m;
    }
};

class ParkingSystem {
private:
    int capacity;
    stack<Car> parkingSlots;
    queue<Car> waitingQueue;
    HistoryList history;

public:
    ParkingSystem(int c) { capacity = c; }

    void arrive(int id, string model) {
        if (parkingSlots.size() < capacity) {
            parkingSlots.push(Car(id, model));
            cout << "Car " << id << " parked successfully.\n";
            history.addHistory(id, model, "Entered Parking");
        } else {
            waitingQueue.push(Car(id, model));
            cout << "Parking Full! Car " << id << " added to waiting queue.\n";
            history.addHistory(id, model, "Added to Waiting Queue");
        }
    }

    void depart() {
        if (parkingSlots.empty()) {
            cout << "Parking is empty, no cars to remove.\n";
            return;
        }
        Car leavingCar = parkingSlots.top();
        parkingSlots.pop();
        cout << "Car " << leavingCar.id << " has exited the parking.\n";
        history.addHistory(leavingCar.id, leavingCar.model, "Exited Parking");

        if (!waitingQueue.empty()) {
            Car nextCar = waitingQueue.front();
            waitingQueue.pop();
            parkingSlots.push(nextCar);
            cout << "Car " << nextCar.id << " moved from queue to parking.\n";
            history.addHistory(nextCar.id, nextCar.model, "Moved Queue -> Parking");
        }
    }

    void displayStatus() {
        cout << "\n========== Parking Status ==========\n";
        cout << "Parked cars: " << parkingSlots.size() << "/" << capacity << endl;
        stack<Car> temp = parkingSlots;
        while (!temp.empty()) {
            Car c = temp.top();
            cout << "- ID: " << c.id << " | Model: " << c.model << endl;
            temp.pop();
        }
        cout << "\nCars in waiting queue: " << waitingQueue.size() << endl;
        queue<Car> tempQ = waitingQueue;
        while (!tempQ.empty()) {
            Car c = tempQ.front();
            cout << "- ID: " << c.id << " | Model: " << c.model << endl;
            tempQ.pop();
        }
        cout << "=====================================\n";
    }

    void showHistory() { history.displayHistory(); }
};

int main() {
    int capacity;
    cout << "Enter parking capacity: ";
    cin >> capacity;
    ParkingSystem parking(capacity);
    int choice;
    while (true) {
        cout << "\n1. Arrive\n2. Depart\n3. Status\n4. History\n5. Exit\nChoice: ";
        cin >> choice;
        if (choice == 1) {
            int id; string model;
            cout << "ID: "; cin >> id;
            cout << "Model: "; cin.ignore(); getline(cin, model);
            parking.arrive(id, model);
        } else if (choice == 2) { parking.depart(); }
        else if (choice == 3) { parking.displayStatus(); }
        else if (choice == 4) { parking.showHistory(); }
        else if (choice == 5) break;
    }
    return 0;
}
