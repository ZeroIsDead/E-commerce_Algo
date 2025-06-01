#include "MatchScheduling.h"
#include <iostream>

using namespace std;

void displayMenu() {
    cout << "\n===============================================" << endl;
    cout << "   APUEC MATCH SCHEDULING & PLAYER PROGRESSION" << endl;
    cout << "===============================================" << endl;
    cout << "1.  Load Players from CSV" << endl;
    cout << "2.  Display All Players" << endl;
    cout << "3.  Create Qualifier Matches" << endl;
    cout << "4.  Create Group Stage Matches" << endl;
    cout << "5.  Create Knockout Matches" << endl;
    cout << "6.  Display Match Queue" << endl;
    cout << "7.  Play All Scheduled Matches" << endl;
    cout << "8.  Display Tournament Bracket" << endl;
    cout << "9.  Eliminate Weak Players" << endl;
    cout << "10. Display Advancing Players" << endl;
    cout << "11. Display Tournament Statistics" << endl;
    cout << "12. Find Tournament Champion" << endl;
    cout << "13. Run Complete Tournament Simulation" << endl;
    cout << "0.  Exit" << endl;
    cout << "===============================================" << endl;
    cout << "Enter your choice: ";
}

void runCompleteTournament(TournamentManager& tournament) {
    cout << "\n🎮 RUNNING COMPLETE APUEC TOURNAMENT SIMULATION 🎮" << endl;
    cout << "====================================================" << endl;
    
    // Step 1: Load players
    cout << "\n>>> STEP 1: Loading Players..." << endl;
    if (!tournament.loadPlayersFromFile("player")) {
        cout << "Failed to load players. Exiting simulation." << endl;
        return;
    }
    tournament.displayPlayers();
    
    // Step 2: Qualifier Stage
    cout << "\n>>> STEP 2: QUALIFIER STAGE" << endl;
    tournament.createQualifierMatches();
    tournament.displayMatchQueue();
    tournament.playAllMatches();
    tournament.eliminateWeakPlayers(1); // Eliminate players with 1+ losses
    tournament.displayTournamentStats();
    
    // Step 3: Group Stage
    cout << "\n>>> STEP 3: GROUP STAGE" << endl;
    tournament.createGroupMatches();
    tournament.playAllMatches();
    tournament.eliminateWeakPlayers(2); // Eliminate players with 2+ losses
    tournament.displayTournamentStats();
    
    // Step 4: Knockout Stage
    cout << "\n>>> STEP 4: KNOCKOUT STAGE" << endl;
    tournament.createKnockoutMatches();
    tournament.displayAdvancingPlayers();
    tournament.playAllMatches();
    
    // Step 5: Final Results
    cout << "\n>>> STEP 5: FINAL RESULTS" << endl;
    tournament.displayTournamentBracket();
    tournament.displayPlayers();
    tournament.findChampion();
    tournament.displayTournamentStats();
    
    cout << "\n🏆 TOURNAMENT SIMULATION COMPLETE! 🏆" << endl;
}

int main() {
    TournamentManager tournament;
    int choice;
    
    cout << "🎮 Welcome to Asia Pacific University Esports Championship! 🎮" << endl;
    cout << "Task 1: Match Scheduling & Player Progression System" << endl;
    
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                string filename;
                cout << "Enter CSV filename (without .csv extension): ";
                cin >> filename;
                tournament.loadPlayersFromFile(filename);
                break;
            }
            
            case 2:
                tournament.displayPlayers();
                break;
                
            case 3:
                tournament.createQualifierMatches();
                cout << "Qualifier matches created successfully!" << endl;
                break;
                
            case 4:
                tournament.createGroupMatches();
                cout << "Group stage matches created successfully!" << endl;
                break;
                
            case 5:
                tournament.createKnockoutMatches();
                cout << "Knockout matches created successfully!" << endl;
                break;
                
            case 6:
                tournament.displayMatchQueue();
                break;
                
            case 7:
                tournament.playAllMatches();
                cout << "All scheduled matches completed!" << endl;
                break;
                
            case 8:
                tournament.displayTournamentBracket();
                break;
                
            case 9: {
                int threshold;
                cout << "Enter elimination threshold (number of losses): ";
                cin >> threshold;
                tournament.eliminateWeakPlayers(threshold);
                break;
            }
            
            case 10:
                tournament.displayAdvancingPlayers();
                break;
                
            case 11:
                tournament.displayTournamentStats();
                break;
                
            case 12:
                tournament.findChampion();
                break;
                
            case 13:
                runCompleteTournament(tournament);
                break;
                
            case 0:
                cout << "Thank you for using APUEC Tournament Management System!" << endl;
                break;
                
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}