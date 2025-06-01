#include <iostream>
#include <fstream>
#include <string>
#include <random>
using namespace std;

// Utility class for common operations
class Utils {
public:
    static string extractColumn(const string& line, int colIndex) {
        string result = "";
        int currentCol = 0;
        
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') {
                if (currentCol == colIndex) break;
                currentCol++;
                result = "";
            } else if (currentCol == colIndex && line[i] != '"' && line[i] != ' ') {
                result += line[i];
            }
        }
        return result;
    }
    
    static void sortArray(string arr[], int size) {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    string temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
    
    static bool fileExists(const string& filename) {
        ifstream file("../data/" + filename + ".csv");
        return file.good();
    }
};

class FileManager {
private:
    static const int MAX_SIZE = 100;
    
public:
    string readFile(const string& filename) {
        ifstream file("../data/" + filename + ".csv");
        if (!file.is_open()) {
            cout << "Error: Could not open " << filename << ".csv" << endl;
            return "";
        }
        
        string content, line;
        while (getline(file, line)) {
            content += line + "\n";
        }
        file.close();
        return content;
    }
    
    // Generic function to read participants from any file
    int readParticipants(const string& filename, string names[], string status[], 
                        int nameCol = 0, int statusCol = 1, const string& filterStatus = "") {
        string content = readFile(filename);
        if (content.empty()) return 0;
        
        int count = 0;
        string line = "";
        bool skipHeader = true;
        
        for (int i = 0; i < content.length(); i++) {
            if (content[i] == '\n' || i == content.length() - 1) {
                if (i == content.length() - 1 && content[i] != '\n') {
                    line += content[i];
                }
                
                if (skipHeader) {
                    skipHeader = false;
                    line = "";
                    continue;
                }
                
                if (!line.empty()) {
                    string name = Utils::extractColumn(line, nameCol);
                    string stat = Utils::extractColumn(line, statusCol);
                    
                    // Filter logic
                    bool includeParticipant = true;
                    if (!filterStatus.empty()) {
                        includeParticipant = (stat == filterStatus);
                    } else {
                        // Default: exclude withdrawn participants
                        includeParticipant = !(stat == "withdrawn" || stat == "Withdrawn" || stat == "WITHDRAWN");
                    }
                    
                    if (includeParticipant && !name.empty()) {
                        names[count] = name;
                        status[count] = stat;
                        count++;
                    }
                }
                line = "";
            } else {
                line += content[i];
            }
        }
        return count;
    }
    
    // Generic function to write match results
    void writeMatches(const string& filename, string matches[][2], string winners[], 
                     int matchCount, const string& prefix = "M", bool includeByes = false, 
                     string byes[] = nullptr, int byeCount = 0) {
        ofstream file("../data/" + filename + ".csv");
        if (!file.is_open()) {
            cout << "Error: Could not create " << filename << ".csv" << endl;
            return;
        }
        
        file << "Match_ID,Player1,Player2,Winner,Status\n";
        
        for (int i = 0; i < matchCount; i++) {
            file << prefix << (i + 1) << "," << matches[i][0] << "," 
                 << matches[i][1] << "," << winners[i] << ",Completed\n";
        }
        
        if (includeByes && byes != nullptr) {
            for (int i = 0; i < byeCount; i++) {
                file << "B" << (i + 1) << "," << byes[i] << ",BYE," 
                     << byes[i] << ",Auto-Win\n";
            }
        }
        
        file.close();
        cout << filename << ".csv written successfully!" << endl;
    }
    
    // Save tournament state for continuity
    void saveTournamentState(const string& stage, string participants[], int count) {
        ofstream file("../data/tournament_state.csv");
        if (file.is_open()) {
            file << "Stage,Participant\n";
            for (int i = 0; i < count; i++) {
                file << stage << "," << participants[i] << "\n";
            }
            file.close();
        }
    }
    
    // Load tournament state
    int loadTournamentState(const string& targetStage, string participants[]) {
        string names[100], stages[100];
        int count = readParticipants("tournament_state", names, stages, 1, 0);
        
        int resultCount = 0;
        for (int i = 0; i < count; i++) {
            if (stages[i] == targetStage) {
                participants[resultCount] = names[i];
                resultCount++;
            }
        }
        return resultCount;
    }
};

// Random decision maker
class Decider {
private:
    random_device rd;
    mt19937 gen;
    
public:
    Decider() : gen(rd()) {}
    
    string decideWinner(const string& player1, const string& player2) {
        uniform_int_distribution<> dis(0, 1);
        return (dis(gen) == 0) ? player1 : player2;
    }
    
    void decideAllWinners(string matches[][2], string winners[], int matchCount) {
        for (int i = 0; i < matchCount; i++) {
            winners[i] = decideWinner(matches[i][0], matches[i][1]);
            cout << matches[i][0] << " vs " << matches[i][1] << " -> Winner: " << winners[i] << endl;
        }
    }
};

// Main tournament class
class Tournament {
private:
    static const int MAX_SIZE = 100;
    FileManager fileManager;
    Decider decider;
    
    // Generic match arrangement function
    int arrangeRound(string participants[], int participantCount, string matches[][2], 
                    string winners[], string byes[], bool autoDecide = true) {
        if (participantCount < 1) return 0;
        
        int matchCount = participantCount / 2;
        int byeCount = participantCount % 2;
        
        // Create matches
        for (int i = 0; i < matchCount; i++) {
            matches[i][0] = participants[i * 2];
            matches[i][1] = participants[i * 2 + 1];
        }
        
        // Handle bye
        if (byeCount > 0) {
            byes[0] = participants[participantCount - 1];
        }
        
        // Decide winners if requested
        if (autoDecide) {
            decider.decideAllWinners(matches, winners, matchCount);
        }
        
        return matchCount;
    }
    
public:
    void runInitialMatches() {
        cout << "\n=== INITIAL MATCHES ===" << endl;
        
        string names[MAX_SIZE], status[MAX_SIZE];
        int count = fileManager.readParticipants("players", names, status, 0, 7);
        
        if (count == 0) {
            cout << "No active participants found!" << endl;
            return;
        }
        
        Utils::sortArray(names, count);
        
        string matches[50][2], winners[50], byes[10];
        int matchCount = arrangeRound(names, count, matches, winners, byes);
        
        fileManager.writeMatches("matchresult", matches, winners, matchCount, "M", true, byes, count % 2);
        
        cout << "Initial matches completed!" << endl;
    }
    
void runGroupStage() {
    cout << "\n=== GROUP STAGE ===" << endl;
    
    // Get winners from initial matches
    string participants[MAX_SIZE];
    string temp[MAX_SIZE];
    
    // Simply read winners from matchresult.csv (column 3 = Winner)
    int count = fileManager.readParticipants("matchresult", participants, temp, 3, 4);
    
    cout << "Total participants for group stage: " << count << endl;
    for (int i = 0; i < count; i++) {
        cout << "- " << participants[i] << endl;
    }
    
    if (count < 4) {
        cout << "Need at least 4 participants for group stage!" << endl;
        return;
    }
    
    int membersPerGroup;
    cout << "Enter members per group (2-" << count/2 << "): ";
    cin >> membersPerGroup;
    
    int groupCount = (count + membersPerGroup - 1) / membersPerGroup;
    
    // Create groups
    string groups[10][10];
    int groupSizes[10] = {0};
    
    for (int i = 0; i < count; i++) {
        int groupIndex = i % groupCount;
        groups[groupIndex][groupSizes[groupIndex]++] = participants[i];
    }
    
    // Display groups
    cout << "\nGroups formed:" << endl;
    for (int i = 0; i < groupCount; i++) {
        cout << "Group " << (i + 1) << ": ";
        for (int j = 0; j < groupSizes[i]; j++) {
            cout << groups[i][j] << (j < groupSizes[i] - 1 ? ", " : "");
        }
        cout << endl;
    }
    
    // Group battles
    string groupMatches[20][2], groupWinners[20];
    int groupMatchCount = 0;
    
    for (int i = 0; i < groupCount - 1; i++) {
        for (int j = i + 1; j < groupCount; j++) {
            groupMatches[groupMatchCount][0] = "G" + to_string(i + 1);
            groupMatches[groupMatchCount][1] = "G" + to_string(j + 1);
            groupMatchCount++;
        }
    }
    
    decider.decideAllWinners(groupMatches, groupWinners, groupMatchCount);
    fileManager.writeMatches("group_results", groupMatches, groupWinners, groupMatchCount, "GB");
    
    // Find winning group
    int groupWins[10] = {0};
    for (int i = 0; i < groupMatchCount; i++) {
        int groupNum = stoi(groupWinners[i].substr(1)) - 1;
        groupWins[groupNum]++;
    }
    
    int winningGroup = 0;
    for (int i = 1; i < groupCount; i++) {
        if (groupWins[i] > groupWins[winningGroup]) {
            winningGroup = i;
        }
    }
    
    cout << "\nWinning Group: G" << (winningGroup + 1) << endl;
    
    // Save winning group members for knockout
    fileManager.saveTournamentState("group_winners", groups[winningGroup], groupSizes[winningGroup]);
    
    cout << "Group stage completed!" << endl;
}
    
    void runKnockoutStage() {
        cout << "\n=== KNOCKOUT STAGE ===" << endl;
        
        // Try to get group winners first from tournament state, then from group results
        string participants[MAX_SIZE];
        int count = fileManager.loadTournamentState("group_winners", participants);
        
        if (count == 0) {
            cout << "No group winners found. Trying to get initial match winners..." << endl;
            // Get winners from initial matches
            string temp[MAX_SIZE];
            count = fileManager.readParticipants("matchresult", participants, temp, 3, 4);
            
            // Also check for BYE winners
            string content = fileManager.readFile("matchresult");
            if (!content.empty()) {
                string line = "";
                bool skipHeader = true;
                
                for (int i = 0; i < content.length(); i++) {
                    if (content[i] == '\n' || i == content.length() - 1) {
                        if (i == content.length() - 1 && content[i] != '\n') {
                            line += content[i];
                        }
                        
                        if (skipHeader) {
                            skipHeader = false;
                            line = "";
                            continue;
                        }
                        
                        if (!line.empty()) {
                            string player2 = Utils::extractColumn(line, 2);
                            string winner = Utils::extractColumn(line, 3);
                            
                            if (player2 == "BYE" && !winner.empty()) {
                                // Check if not already added
                                bool alreadyExists = false;
                                for (int j = 0; j < count; j++) {
                                    if (participants[j] == winner) {
                                        alreadyExists = true;
                                        break;
                                    }
                                }
                                if (!alreadyExists) {
                                    participants[count] = winner;
                                    count++;
                                }
                            }
                        }
                        line = "";
                    } else {
                        line += content[i];
                    }
                }
            }
        }
        
        if (count <= 1) {
            if (count == 1) {
                cout << "Tournament Champion: " << participants[0] << endl;
            } else {
                cout << "No participants found for knockout!" << endl;
            }
            return;
        }
        
        int round = 1;
        string currentParticipants[MAX_SIZE];
        for (int i = 0; i < count; i++) {
            currentParticipants[i] = participants[i];
        }
        int currentCount = count;
        
        while (currentCount > 1) {
            cout << "\n--- ROUND " << round << " ---" << endl;
            
            string matches[25][2], winners[25], byes[5];
            int matchCount = arrangeRound(currentParticipants, currentCount, matches, winners, byes);
            
            // Prepare next round participants
            int nextCount = 0;
            for (int i = 0; i < matchCount; i++) {
                currentParticipants[nextCount++] = winners[i];
            }
            if (currentCount % 2 > 0) {
                currentParticipants[nextCount++] = byes[0];
            }
            
            currentCount = nextCount;
            round++;
            
            if (currentCount == 1) {
                cout << "\n🏆 TOURNAMENT CHAMPION: " << currentParticipants[0] << " 🏆" << endl;
                
                // Save champion
                ofstream champFile("../data/champion.txt");
                if (champFile.is_open()) {
                    champFile << currentParticipants[0] << endl;
                    champFile.close();
                }
                break;
            }
        }
    }
    
    void viewResults(const string& filename) {
        string content = fileManager.readFile(filename);
        if (!content.empty()) {
            cout << "\n=== " << filename << " ===" << endl;
            cout << content << endl;
        } else {
            cout << "No results found in " << filename << ".csv" << endl;
        }
    }
    
    void viewChampion() {
        ifstream file("../data/champion.txt");
        if (file.is_open()) {
            string champion;
            getline(file, champion);
            cout << "\n TOURNAMENT CHAMPION: " << champion << endl;
            file.close();
        } else {
            cout << "No champion found yet!" << endl;
        }
    }
};

// Menu system
class Menu {
private:
    Tournament tournament;
    
public:
    void display() {
        cout << "\n========== TOURNAMENT SYSTEM ==========" << endl;
        cout << "1. Run Initial Matches" << endl;
        cout << "2. Run Group Stage" << endl;
        cout << "3. Run Knockout Stage" << endl;
        cout << "4. View Initial Results" << endl;
        cout << "5. View Group Results" << endl;
        cout << "6. View Champion" << endl;
        cout << "7. Exit" << endl;
        cout << "=======================================" << endl;
        cout << "Choice: ";
    }
    
    void run() {
        int choice;
        while (true) {
            display();
            cin >> choice;
            
            switch (choice) {
                case 1: tournament.runInitialMatches(); break;
                case 2: tournament.runGroupStage(); break;
                case 3: tournament.runKnockoutStage(); break;
                case 4: tournament.viewResults("matchresult"); break;
                case 5: tournament.viewResults("group_results"); break;
                case 6: tournament.viewChampion(); break;
                case 7: 
                    cout << "Goodbye!" << endl;
                    return;
                default:
                    cout << "Invalid choice!" << endl;
            }
            
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
};

int main() {
    cout << "Welcome to Tournament Management System!" << endl;
    Menu menu;
    menu.run();
}