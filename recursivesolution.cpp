/* File Name: recursivesolution.cpp
 * Description: Contains the function that implements the
 * solution to the problem recursively, and test cases.
 */

#include <iostream>
#include "activity.h"
#include "set.h"
#include "map.h"
#include "testing/SimpleTest.h"
using namespace std;

/* Function Name: scheduleRecursive
 *
 * Description: Overloads the given scheduleRecursive function prototype.
 * Given a list of activities, map of activities that must be completed before
 * a given activity, and time limit, finds the number of possible orderings of
 * activities and prints them out. This is the recursive approach.
 *
 * Parameters: Vector<Activity>& soFar, activities chosen so far,
 * Vector<Activity>& activities, activities that have not been chosen yet,
 * Map<string, Set<Activity>>& prerequisites, maps the name of an activity to the set
 * of activities that must be completed before it,
 * int remaining, the number of minutes remaining after the activities chosen so far
 *
 * Return: int, the number of possible orderings
 */
int scheduleRecursive(Vector<Activity>& soFar, Vector<Activity>& activities, Map<string, Set<Activity>>& prerequisites, int remaining) {
    bool done = true;

    // check if any activities can be added without exceeding time limit
    for (Activity a : activities) {
        if (a.time <= remaining) {
            done = false;
        }
    }

    // base case: no more activities can be added
    if (done || activities.isEmpty()) {
        // not an ordering, do not count
        if (soFar.isEmpty()) {
            return 0;
        }

        // ordering found
        for (Activity a : soFar) {
            cout << a << endl;
        }
        cout << endl;
        return 1;
    }

    // recursive case
    int result = 0;

    for (int i = 0; i < activities.size(); i++) {
        Activity curr = activities[i];
        bool allowed = true;

        // check if the activity time will not exceed the limit
        if (curr.time > remaining) {
            allowed = false;
        }
        // check if the activity's prerequisites have been chosen already
        else {
            for (Activity a : prerequisites[curr.name]) {
                if (!soFar.contains(a)) {
                    allowed = false;
                }
            }
        }

        if (allowed) {
            // choose
            soFar.add(curr);
            activities.remove(i);
            remaining -= curr.time;

            // explore
            result += scheduleRecursive(soFar, activities, prerequisites, remaining);

            // unchoose
            soFar.remove(soFar.size() - 1);
            activities.insert(i, curr);
            remaining += curr.time;
        }
    }
    return result;
}

/* Function Name: scheduleRecursive
 *
 * Description: Initializes an empty soFar Vector and calls the overloaded
 * helper function to return the number of possible orderings and print the
 * orderings.
 *
 * Parameters: Vector<Activity>& activities, activities that can be done,
 * Map<string, Set<Activity>>& prerequisites, maps the name of an activity to the set
 * of activities that must be completed before it,
 * int time, the total amount of time allowed in minutes for an ordering of activities
 *
 * Return: int, the number of possible orderings
 */
int scheduleRecursive(Vector<Activity>& activities, Map<string, Set<Activity>>& prerequisites, int time) {
    Vector<Activity> soFar;
    return scheduleRecursive(soFar, activities, prerequisites, time);
}

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("Small tests and edge cases"){
    Activity a1 = {"wash dishes", 5};
    Activity a2 = {"brush hair", 5};
    Activity a3 = {"shower", 10};

    cout << "No restrictions" << endl;
    Vector<Activity> activities = {a1, a2, a3};
    Map<string, Set<Activity>> prerequisites;
    int time = 50;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 6);

    cout << "Prereq restriction" << endl;
    activities = {a1, a2, a3};
    prerequisites[a1.name] = {a2, a3};
    time = 50;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 2);

    cout << "Impossible prereqs" << endl;
    activities = {a1, a2, a3};
    prerequisites[a1.name] = {a2};
    prerequisites[a2.name] = {a3};
    prerequisites[a3.name] = {a1};
    time = 100;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 0);

    cout << "Time restriction" << endl;
    activities = {a1, a2, a3};
    prerequisites.clear();
    time = 10;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 3);

    cout << "Prereq and time restriction" << endl;
    activities = {a1, a2, a3};
    prerequisites.clear();
    prerequisites[a1.name] = {a2, a3};
    time = 18;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 2);

    cout << "Prereq and time restriction" << endl;
    activities = {a1, a2, a3};
    prerequisites.clear();
    prerequisites[a1.name] = {a2};
    time = 10;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 2);

    cout << "No time" << endl;
    activities = {a1, a2, a3};
    prerequisites.clear();
    time = 2;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 0);
}

STUDENT_TEST("No activities case and larger tests"){
    cout << "No activities" << endl;
    Vector<Activity> activities;
    Map<string, Set<Activity>> prerequisites;
    int time = 50;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 0);

    Activity so1 = {"sock1", 1};
    Activity sh1 = {"shoe1", 1};
    Activity so2 = {"sock2", 1};
    Activity sh2 = {"shoe2", 1};
    Activity so3 = {"sock3", 1};
    Activity sh3 = {"shoe3", 1};
    activities = {so1, sh1, so2, sh2, so3, sh3};
    prerequisites[sh1.name] = {so1};
    prerequisites[sh2.name] = {so2};
    prerequisites[sh3.name] = {so3};
    time = 100;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 90);

    activities = {so1, sh1, so2, sh2, so3, sh3};
    time = 2;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 9);

    activities = {so1, sh1, so2, sh2, so3, sh3};
    time = 4;
    EXPECT_EQUAL(scheduleRecursive(activities, prerequisites, time), 54);
}
