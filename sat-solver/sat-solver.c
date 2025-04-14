#include "libmin.h"

#define NUM_VARS     8      // Number of variables: x1, x2, ..., x8
#define NUM_CLAUSES  10     // Total number of clauses in the CNF formula
#define MAX_LITERALS 4      // Maximum literals per clause (including terminating 0)
#define UNASSIGNED   -1     // Marker for unassigned variables

// Define the medium-hard CNF formula.
// Each clause is represented as an array terminated by 0.
static int formula[NUM_CLAUSES][MAX_LITERALS] = {
    {  1,  2,  3, 0 },   // Clause 1: (x1 ∨ x2 ∨ x3)
    { -1,  4,  0, 0 },   // Clause 2: (¬x1 ∨ x4)
    { -2,  4,  0, 0 },   // Clause 3: (¬x2 ∨ x4)
    { -3,  4,  0, 0 },   // Clause 4: (¬x3 ∨ x4)
    { -4,  5,  6, 0 },   // Clause 5: (¬x4 ∨ x5 ∨ x6)
    { -5,  7,  0, 0 },   // Clause 6: (¬x5 ∨ x7)
    { -6,  7,  0, 0 },   // Clause 7: (¬x6 ∨ x7)
    { -7,  8,  0, 0 },   // Clause 8: (¬x7 ∨ x8)
    {  2, -6,  8, 0 },   // Clause 9: (x2 ∨ ¬x6 ∨ x8)
    { -3, -8,  0, 0 }    // Clause 10: (¬x3 ∨ ¬x8)
};

// Array to store the truth assignment for each variable (1-indexed).
// A value of UNASSIGNED means the variable has not yet been given a truth value.
static int assignment[NUM_VARS + 1];

// Function to check if the current assignment causes a conflict.
// For each clause, if all literals are assigned and none is true, then a conflict exists.
int conflict() {
    for (int i = 0; i < NUM_CLAUSES; i++) {
        int satisfied = 0;
        int fully_assigned = 1;
        for (int j = 0; j < MAX_LITERALS && formula[i][j] != 0; j++) {
            int lit = formula[i][j];
            int var = (lit > 0) ? lit : -lit;
            int val = assignment[var];
            if (val == UNASSIGNED) {
                fully_assigned = 0;
            } else {
                // For a positive literal, it is satisfied if the value is 1;
                // for a negative literal, it is satisfied if the value is 0.
                if ((lit > 0 && val == 1) || (lit < 0 && val == 0)) {
                    satisfied = 1;
                    break;  // This clause is satisfied.
                }
            }
        }
        // If a clause is fully assigned and not satisfied, there is a conflict.
        if (!satisfied && fully_assigned)
            return 1;
    }
    return 0;
}

// Recursive backtracking solver.
// 'var' is the current variable index to assign (starting at 1).
// Returns 1 if a satisfying assignment is found.
int solveSAT(int var) {
    if (var > NUM_VARS) {
        // All variables are assigned.
        return conflict() ? 0 : 1;
    }
    // Try both truth values: 0 (false) and 1 (true).
    for (int v = 0; v < 2; v++) {
        assignment[var] = v;
        if (!conflict() && solveSAT(var + 1))
            return 1;
    }
    assignment[var] = UNASSIGNED;  // Backtrack.
    return 0;
}

// Utility to print the CNF formula.
void printFormula() {
    libmin_printf("CNF Formula:\n");
    for (int i = 0; i < NUM_CLAUSES; i++) {
        libmin_printf("(");
        for (int j = 0; j < MAX_LITERALS && formula[i][j] != 0; j++) {
            int lit = formula[i][j];
            if (lit < 0)
                libmin_printf("¬x%d ", -lit);
            else
                libmin_printf("x%d ", lit);
        }
        libmin_printf(")\n");
    }
}

// Utility to print the variable assignments once a solution is found.
void printAssignment() {
    libmin_printf("SAT Solution (Variable Assignment):\n");
    for (int i = 1; i <= NUM_VARS; i++) {
        libmin_printf("x%d = %s\n", i, (assignment[i] == 1 ? "true" : "false"));
    }
}

int main() {
    // Initialize all variables to UNASSIGNED.
    for (int i = 1; i <= NUM_VARS; i++) {
        assignment[i] = UNASSIGNED;
    }
    
    printFormula();

    if (solveSAT(1)) {
        libmin_printf("SAT solution found:\n");
        printAssignment();
        libtarg_success();
    } else {
        libmin_printf("No SAT solution exists.\n");
        libtarg_fail(1);
    }
    return 0;
}

