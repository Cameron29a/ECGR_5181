//******************************************************************************
// Libraries
//******************************************************************************
#include <iostream>
#include <ctime>
#include <bits/stdc++.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <stdio.h>
#include <cstdlib>

using namespace std;

typedef uint64_t tick_t;

//******************************************************************************
// Defines
//******************************************************************************
#define NUM_EVENTS 20
#define MAX_EVENTS 100


//******************************************************************************
// Function Prototypes
//******************************************************************************
int seedRNG(int);
vector<pair<int, int>> insertionSort(vector<pair<int, int>>);
void printMEQ(vector<pair<int, int>>);
vector<pair<int, int>> addNewEvent(vector<pair<int, int>>);


//***********************************************************************************
// Driver Code
//***********************************************************************************
int main() {
    vector<pair<int, int>> events;  // Vector to store events
    vector<pair<int, int>> processedEvents;
    tick_t t;
    int v;
    srand((unsigned) clock());
    
    // Generate random number and add event to vector
    for (int i = 0; i < NUM_EVENTS; ++i) {
        t = clock();
        v = rand() % 100;
        events.push_back(make_pair(t, v));
    }

    // Print the generated events
    cout << "\nUnsorted list:\n";
    printMEQ(events);
    
    // Sort generated events
    events = insertionSort(events);

    // Print the sorted generated events
    cout << "\nSorted list:\n";
    printMEQ(events);
    
    // Add events and sort MEQ until 100 events have been processed
    while(processedEvents.size() < MAX_EVENTS){
        time_t tt = events[0].first;
        int vv = events[0].second;
        processedEvents.push_back(make_pair(tt, vv));
        // events.pop_back();
        events.erase(events.begin());
        events = addNewEvent(events);
        events = insertionSort(events);
    }
    cout << "\nSorted list:\n";
    printMEQ(processedEvents);
    
    return 0;
}

//***********************************************************************************
// Functions
//***********************************************************************************
//******************************************************************************
// RNG - using passed int as seed
//******************************************************************************
int seedRNG(int n) {
    srand((unsigned) n);
    return rand() % 100;
}

//******************************************************************************
// Insertion Sort
//******************************************************************************
vector<pair<int, int>> insertionSort(vector<pair<int, int>> events) {
    int n = events.size();
    for (int j = 1; j < n; j++) {
        pair<int, int> temp = events[j];
        int i = j - 1;
        
        while (i >= 0 && (events[i].first > temp.first || (events[i].first == temp.first && events[i].second > temp.second))) {
            events[i + 1] = events[i];
            i = i - 1;
        }
        events[i + 1] = temp;
    }
    return events;
}

//******************************************************************************
// Prints out the Master Event Queue(MEQ)
//******************************************************************************
void printMEQ(vector<pair<int, int>> events) {
    int n = events.size();
    for (int i = 0; i < n; i++)
        cout << "Event "<< i << ": E = (" << events[i].first << ", " << events[i].second << ")\n";

    cout << "Size of events vector: " << events.size() << "\n\n";
}

//******************************************************************************
// Adds new event to MEQ
//******************************************************************************
vector<pair<int, int>> addNewEvent(vector<pair<int, int>> events) {
    int t = events[0].first;
    int v = events[0].second;

    cout << "Earliest event: v = " << v << "\n";
    
    int tt = t + seedRNG(v);
    events.push_back(make_pair(tt, v));
    
    return events;
}
