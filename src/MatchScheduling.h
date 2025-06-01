#ifndef MATCHSCHEDULING_H
#define MATCHSCHEDULING_H

#include "ArrayHeader.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Structure to represent a player/team
struct Player {
    string groupID;
    string groupName;
    int ranking;
    bool isActive;
    int wins;
    int losses;
    
    Player() : ranking(0), isActive(true), wins(0), losses(0) {}
    
    Player(string id, string name, int rank = 0) 
        : groupID(id), groupName(name), ranking(rank), isActive(true), wins(0), losses(0) {}
};

// Structure to represent a match
struct Match {
    string matchID;
    Player player1;
    Player player2;
    string winner;
    string stage; // "qualifier", "group", "knockout"
    bool isCompleted;
    string result;
    
    Match() : isCompleted(false) {}
    
    Match(string id, Player p1, Player p2, string stg) 
        : matchID(id), player1(p1), player2(p2), stage(stg), isCompleted(false) {}
};

// Stack implementation for match progression tracking
template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T value) : data(value), next(nullptr) {}
    };
    
    Node* top;
    int size;
    
public:
    Stack() : top(nullptr), size(0) {}
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    void push(T item) {
        Node* newNode = new Node(item);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    T pop() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        Node* temp = top;
        T data = top->data;
        top = top->next;
        delete temp;
        size--;
        return data;
    }
    
    T peek() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        return top->data;
    }
    
    bool isEmpty() {
        return top == nullptr;
    }
    
    int getSize() {
        return size;
    }
    
    void display() {
        Node* current = top;
        cout << "Stack contents (top to bottom):" << endl;
        while (current != nullptr) {
            cout << "- " << current->data.groupName << " (" << current->data.groupID << ")" << endl;
            current = current->next;
        }
    }
};

// Queue implementation for match scheduling
template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T value) : data(value), next(nullptr) {}
    };
    
    Node* front;
    Node* rear;
    int size;
    
public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}
    
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    void enqueue(T item) {
        Node* newNode = new Node(item);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }
    
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        Node* temp = front;
        T data = front->data;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        size--;
        return data;
    }
    
    T getFront() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return front->data;
    }
    
    bool isEmpty() {
        return front == nullptr;
    }
    
    int getSize() {
        return size;
    }
    
    void display() {
        Node* current = front;
        cout << "Queue contents (front to rear):" << endl;
        while (current != nullptr) {
            cout << "- Match: " << current->data.player1.groupName 
                 << " vs " << current->data.player2.groupName 
                 << " (" << current->data.stage << ")" << endl;
            current = current->next;
        }
    }
};

// Main Tournament Management Class
class TournamentManager {
private:
    Functions fileHandler;
    Player* players;
    int playerCount;
    Stack<Player> advancingPlayers;  // Players advancing to next stage
    Queue<Match> matchQueue;         // Scheduled matches waiting to be played
    Match* completedMatches;
    int completedMatchCount;
    int maxMatches;
    
    // Generate unique match ID
    string generateMatchID(string stage, int matchNum) {
        return stage.substr(0, 3) + "_" + to_string(matchNum);
    }
    
    // Shuffle players array for random seeding
    void shufflePlayers() {
        srand(time(nullptr));
        for (int i = playerCount - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            Player temp = players[i];
            players[i] = players[j];
            players[j] = temp;
        }
    }
    
public:
    TournamentManager() : players(nullptr), playerCount(0), completedMatches(nullptr), 
                         completedMatchCount(0), maxMatches(100) {
        completedMatches = new Match[maxMatches];
    }
    
    ~TournamentManager() {
        delete[] players;
        delete[] completedMatches;
    }
    
    // Load players from CSV file using ArrayHeader.h functionality
    bool loadPlayersFromFile(string filename) {
        DataContainer2d data = fileHandler.getdata(filename);
        
        if (data.error == 1) {
            cout << "Error loading player data from file!" << endl;
            return false;
        }
        
        playerCount = data.y;
        players = new Player[playerCount];
        
        // Load players and assign random rankings
        srand(time(nullptr));
        for (int i = 0; i < playerCount; i++) {
            players[i] = Player(data.data[i][0], data.data[i][1], rand() % 1000 + 1);
        }
        
        cout << "Successfully loaded " << playerCount << " players from " << filename << ".csv" << endl;
        return true;
    }
    
    // Display all players with their current stats
    void displayPlayers() {
        cout << "\n=== TOURNAMENT PLAYERS ===" << endl;
        cout << "ID\t\tName\t\t\tRanking\tWins\tLosses\tStatus" << endl;
        cout << "----------------------------------------------------------------" << endl;
        for (int i = 0; i < playerCount; i++) {
            cout << players[i].groupID << "\t\t" 
                 << players[i].groupName << "\t\t" 
                 << players[i].ranking << "\t" 
                 << players[i].wins << "\t" 
                 << players[i].losses << "\t"
                 << (players[i].isActive ? "Active" : "Eliminated") << endl;
        }
    }
    
    // Create matches for qualifier stage
    void createQualifierMatches() {
        cout << "\n=== CREATING QUALIFIER MATCHES ===" << endl;
        
        // Shuffle players for fair random pairing
        shufflePlayers();
        
        int matchNum = 1;
        for (int i = 0; i < playerCount - 1; i += 2) {
            if (players[i].isActive && players[i + 1].isActive) {
                Match newMatch(generateMatchID("QUAL", matchNum), 
                              players[i], players[i + 1], "qualifier");
                matchQueue.enqueue(newMatch);
                matchNum++;
            }
        }
        
        cout << "Created " << matchQueue.getSize() << " qualifier matches." << endl;
    }
    
    // Create matches for group stage
    void createGroupMatches() {
        cout << "\n=== CREATING GROUP STAGE MATCHES ===" << endl;
        
        // Get active players and create round-robin within groups
        Player* activePlayers = new Player[playerCount];
        int activeCount = 0;
        
        for (int i = 0; i < playerCount; i++) {
            if (players[i].isActive) {
                activePlayers[activeCount++] = players[i];
            }
        }
        
        int matchNum = 1;
        // Create matches between every pair of active players
        for (int i = 0; i < activeCount - 1; i++) {
            for (int j = i + 1; j < activeCount; j++) {
                Match newMatch(generateMatchID("GRP", matchNum), 
                              activePlayers[i], activePlayers[j], "group");
                matchQueue.enqueue(newMatch);
                matchNum++;
            }
        }
        
        delete[] activePlayers;
        cout << "Created " << matchQueue.getSize() << " group stage matches." << endl;
    }
    
    // Create knockout bracket matches
    void createKnockoutMatches() {
        cout << "\n=== CREATING KNOCKOUT MATCHES ===" << endl;
        
        // Push all active players to stack for bracket organization
        for (int i = 0; i < playerCount; i++) {
            if (players[i].isActive) {
                advancingPlayers.push(players[i]);
            }
        }
        
        int matchNum = 1;
        // Create knockout matches by pairing players from stack
        while (advancingPlayers.getSize() >= 2) {
            Player p1 = advancingPlayers.pop();
            Player p2 = advancingPlayers.pop();
            
            Match newMatch(generateMatchID("KO", matchNum), p1, p2, "knockout");
            matchQueue.enqueue(newMatch);
            matchNum++;
        }
        
        cout << "Created " << matchQueue.getSize() << " knockout matches." << endl;
    }
    
    // Simulate a match and determine winner
    void simulateMatch(Match& match) {
        srand(time(nullptr) + rand());
        
        // Simple simulation based on ranking (higher ranking has better chance)
        int p1Score = match.player1.ranking + (rand() % 100);
        int p2Score = match.player2.ranking + (rand() % 100);
        
        if (p1Score > p2Score) {
            match.winner = match.player1.groupID;
            match.result = match.player1.groupName + " defeats " + match.player2.groupName;
            
            // Update player stats
            for (int i = 0; i < playerCount; i++) {
                if (players[i].groupID == match.player1.groupID) {
                    players[i].wins++;
                } else if (players[i].groupID == match.player2.groupID) {
                    players[i].losses++;
                    if (match.stage == "knockout") {
                        players[i].isActive = false; // Eliminate in knockout
                    }
                }
            }
        } else {
            match.winner = match.player2.groupID;
            match.result = match.player2.groupName + " defeats " + match.player1.groupName;
            
            // Update player stats
            for (int i = 0; i < playerCount; i++) {
                if (players[i].groupID == match.player2.groupID) {
                    players[i].wins++;
                } else if (players[i].groupID == match.player1.groupID) {
                    players[i].losses++;
                    if (match.stage == "knockout") {
                        players[i].isActive = false; // Eliminate in knockout
                    }
                }
            }
        }
        
        match.isCompleted = true;
    }
    
    // Play all matches in the queue
    void playAllMatches() {
        cout << "\n=== PLAYING MATCHES ===" << endl;
        
        while (!matchQueue.isEmpty()) {
            Match currentMatch = matchQueue.dequeue();
            simulateMatch(currentMatch);
            
            // Store completed match
            if (completedMatchCount < maxMatches) {
                completedMatches[completedMatchCount++] = currentMatch;
            }
            
            cout << "Match " << currentMatch.matchID << " (" << currentMatch.stage << "): " 
                 << currentMatch.result << endl;
        }
    }
    
    // Display tournament bracket/progression
    void displayTournamentBracket() {
        cout << "\n=== TOURNAMENT BRACKET ===" << endl;
        
        // Group matches by stage
        cout << "\nQUALIFIER MATCHES:" << endl;
        for (int i = 0; i < completedMatchCount; i++) {
            if (completedMatches[i].stage == "qualifier") {
                cout << "  " << completedMatches[i].matchID << ": " 
                     << completedMatches[i].result << endl;
            }
        }
        
        cout << "\nGROUP STAGE MATCHES:" << endl;
        for (int i = 0; i < completedMatchCount; i++) {
            if (completedMatches[i].stage == "group") {
                cout << "  " << completedMatches[i].matchID << ": " 
                     << completedMatches[i].result << endl;
            }
        }
        
        cout << "\nKNOCKOUT MATCHES:" << endl;
        for (int i = 0; i < completedMatchCount; i++) {
            if (completedMatches[i].stage == "knockout") {
                cout << "  " << completedMatches[i].matchID << ": " 
                     << completedMatches[i].result << endl;
            }
        }
    }
    
    // Eliminate players based on performance (for progression between stages)
    void eliminateWeakPlayers(int eliminationThreshold = 2) {
        cout << "\n=== ELIMINATING PLAYERS ===" << endl;
        int eliminatedCount = 0;
        
        for (int i = 0; i < playerCount; i++) {
            if (players[i].isActive && players[i].losses >= eliminationThreshold) {
                players[i].isActive = false;
                eliminatedCount++;
                cout << "Eliminated: " << players[i].groupName 
                     << " (Losses: " << players[i].losses << ")" << endl;
            }
        }
        
        cout << "Total players eliminated: " << eliminatedCount << endl;
    }
    
    // Display current match queue
    void displayMatchQueue() {
        cout << "\n=== UPCOMING MATCHES ===" << endl;
        if (matchQueue.isEmpty()) {
            cout << "No matches scheduled." << endl;
        } else {
            matchQueue.display();
        }
    }
    
    // Display advancing players stack
    void displayAdvancingPlayers() {
        cout << "\n=== PLAYERS ADVANCING ===" << endl;
        if (advancingPlayers.isEmpty()) {
            cout << "No players in advancement queue." << endl;
        } else {
            advancingPlayers.display();
        }
    }
    
    // Get tournament statistics
    void displayTournamentStats() {
        int activeCount = 0;
        int totalMatches = completedMatchCount;
        
        for (int i = 0; i < playerCount; i++) {
            if (players[i].isActive) {
                activeCount++;
            }
        }
        
        cout << "\n=== TOURNAMENT STATISTICS ===" << endl;
        cout << "Total Players: " << playerCount << endl;
        cout << "Active Players: " << activeCount << endl;
        cout << "Eliminated Players: " << (playerCount - activeCount) << endl;
        cout << "Completed Matches: " << totalMatches << endl;
        cout << "Pending Matches: " << matchQueue.getSize() << endl;
    }
    
    // Find champion (last remaining active player)
    void findChampion() {
        cout << "\n=== TOURNAMENT CHAMPION ===" << endl;
        
        Player* champion = nullptr;
        int activeCount = 0;
        
        for (int i = 0; i < playerCount; i++) {
            if (players[i].isActive) {
                champion = &players[i];
                activeCount++;
            }
        }
        
        if (activeCount == 1 && champion != nullptr) {
            cout << "🏆 CHAMPION: " << champion->groupName 
                 << " (" << champion->groupID << ")" << endl;
            cout << "Final Record: " << champion->wins << " wins, " 
                 << champion->losses << " losses" << endl;
        } else if (activeCount > 1) {
            cout << "Tournament still in progress. " << activeCount 
                 << " players remaining." << endl;
        } else {
            cout << "No active players found!" << endl;
        }
    }
};

#endif