/* Name: Reed Kass-Mullet
 * Date: 2/17/21
 * Class: COMP167
 * Assignment: HW1 - Part 2
 * Description: This program uses the Needleman-Wunsch algorithm to score
 *              a given set of two sequences based on a rubric set in the
 *              main. The program uses a two-dimsional vector as the primary
 *              data structure. 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
using namespace std;


/* struct node is the structure stored in the table which contains all
 * information of what the score at that point is, and where the value
 *  originated from relative to the squares near it.
*/
struct node {
    int score;
    bool left;
    bool top;
    bool diag;
};

/* struct rubic is a partially redundant structure, for user ease, which
 * contains the values used to score the table
 */
struct rubric {
  int gap;
  int mis;
  int match;
};

/* struct table is the data structure used to store all relevant information
 * regarding the dynamic scoring of the table and the re-tracing of the correct
 * sequences.
 */
struct table {
  string seq1;
  string seq2;
  vector<vector<struct node>> table;
};

/* struct pairs is a struct which contains the two values of a given point in
 * the printed optimal alignment. It is used in the print function
 */
struct pairs {
  int seq1;
  int seq2;
};


//Given the small nature of this program, I chose to not bother with a .h file
struct table intitialize();
vector<vector<struct node>> build_table(string seq1, string seq2);
struct table score_table(struct table tab, struct rubric rub);
int score_spot(struct table tab, int i, int j, struct rubric rub);
void print_alignment(struct table tab);
void recursive_print(struct table tab, int size, int i, int j, vector<struct pairs> current);
void print_table(struct table tab);


int main(int argc, char* argv[]) {
  (void) argc;
  (void) argv;

  //Here you can quickly change the scoring rubric for the program
  struct rubric rub;
  rub.gap = -2;
  rub.mis = -2;
  rub.match = 4;
  
  struct table tab = intitialize();
  tab = score_table(tab, rub);
  print_alignment(tab);
  return 0;
}

/* Function: initialize()
 * Parameters: None
 * return: struct table containing a spec initialized data structure
 * Purpose: This function initializes the program by reading in the sequences
 *          and their descriptors. It then loads this data into the table 
 *          struct. It callsa sub function build_table to fill out and
 *          initialize the 2D array.
 */
struct table intitialize() {
  struct table tab;
  string descriptor1;
  string descriptor2;
  string seq1;
  string seq2;
  getline(cin, descriptor1);
  getline(cin, seq1);
  getline(cin, descriptor2);
  getline(cin, seq2);
  tab.table = build_table(seq1, seq2);
  tab.seq1 = seq1;
  tab.seq2 = seq2;
  return tab;
}

/* Function: build_table()
 * Parameters: seq1 & seq2 contain the sequences being analyzed
 * return: two dimensional vector of the size given by the sequences
 * Purpose: This function initializes the two-dimensional array used
 *          to hold all of the relevant information. The sequences
 *          passed in are only used to ascertain their length.
 */
vector<vector<struct node>> build_table(string seq1, string seq2) {
  int cols = seq1.length() + 1;
  int rows = seq2.length() + 1;
  vector<vector<struct node>> table(rows);

  for (int i = 0; i < rows; i++) {
    vector<struct node> new_row(cols);
    table[i] = new_row;
  }
  return table;
}

/* Function: score_table()
 * Parameters: struct table tab, to be scored; struct rubric rub used to score
 * return: struct table tab which has been scored.
 * Purpose: This function scores the table based on the passed in rubric.
 *          It uses a nested for loop to hit every square. It returns the
 *          fully scored table.
 */
struct table score_table(struct table tab, struct rubric rub) {
  for (int i = 0; i < (int) tab.table.size(); i ++) {
    for (int j = 0; j < (int) tab.table[0].size(); j ++) {
       if (i == 0) {
        tab.table[i][j].score = j * rub.gap;
        tab.table[i][j].left = true;
      } else if (j == 0) {
        tab.table[i][j].score = i * rub.gap;
        tab.table[i][j].top = true;
      } else {
        int diag = tab.table[i-1][j-1].score + score_spot(tab, i, j, rub);
        int left = tab.table[i][j-1].score + rub.gap;
        int top = tab.table[i-1][j].score + rub.gap;
        vector<int> max{left, diag, top};
        sort(max.begin(), max.end(), greater<int>());
        int maxnum = max[0];
          if (left == maxnum) {
            tab.table[i][j].left = true;
          }
          if (diag == maxnum) {
            tab.table[i][j].diag = true;
          }
          if (top == maxnum) {
            tab.table[i][j].top = true;
          }
        tab.table[i][j].score = maxnum;
      }
    }
  }
  return tab;
}

/* Function: score_spot()
 * Parameters: the table tab, current indexs i & j, and the rubric
 * return: the score of the position
 * Purpose: This function scores a specific match or mismatch
 *          based on the rubric.
 */
int score_spot(struct table tab, int i, int j, struct rubric rub) {
  if (tab.seq1[j - 1] == tab.seq2[i - 1]) {
    return rub.match;
  } else return rub.mis;
}

/* Function: print_table()
 * Parameters: struct table tab, a scored table
 * return: None
 * Purpose: This function prints a formatted and somewhat pretty
 *          visual of the complete table. It is not called in the
 *          final program, but was helpful for debugging
 */
void print_table(struct table tab) {
  cout << "(--)";
  for(int i = 0; i < (int) tab.table[0].size(); i++) {
    if(i == 0) {
      cout << "(\"_\")";
    } else {
      cout << "(__" << tab.seq1[i - 1] << ")";
    }
  }
  cout << endl;
  for (int i = 0; i < (int) tab.table.size(); i++ ) {
    if(i == 0) {
      cout << "\"_\":";
    } else {
      cout << tab.seq2[i - 1] << ": ";
    }
    for (int j = 0; j < (int) tab.table[0].size(); j++) {
      if(tab.table[i][j].diag) {
        printf("(%3d)", tab.table[i][j].score);
      } else {
          printf("{%3d}", tab.table[i][j].score);
        }
      }
    cout << endl;
  }
}

/* Function: print_alginment()
 * Parameters: struct table tab, a scored table
 * return: None
 * Purpose: This function calls the recursive print function with its
 *          initial parameters.
 */
void print_alignment(struct table tab) {
  vector<struct pairs> curr;
  recursive_print(tab, 0, tab.table.size() - 1, tab.table[0].size() - 1, curr);
}

/* Function: recursive_print()
 * Parameters: struct table tab, a scored table
 * return: None
 * Purpose: This function recursively prints *all* possible matches. However,
 *          given the constains for the homework, and the specific input/output
 *          examples we were given, I have changed it to print the single one based
 *          on the same priorities used in the example print. You can change
 *          this function to print all optimal alignmentsby changing the "else if"'s
 *          on line 253 and 261 to simple "if"'s.
 */
void recursive_print(struct table tab, int size, int i, int j, vector<struct pairs> current) {

  size = size + 1;
  if (i == 0 && j == 0) {
    for (int i = current.size() - 1; i >= 0; i--) {
      cout << (char) current[i].seq1;
    }
    cout << endl;
    for (int i = current.size() - 1; i >= 0; i--) {
      cout << (char) current[i].seq2;
    }
  } else {
    if (tab.table[i][j].diag == true) {
      struct pairs to_push;
      to_push.seq1 = tab.seq1.at(j - 1);
      to_push.seq2 = tab.seq2.at(i - 1);
      current.push_back(to_push);
      recursive_print(tab, size, i-1, j-1, current);
      current.pop_back();
    }
    else if (tab.table[i][j].left == true) {
      struct pairs to_push;
      to_push.seq1 = tab.seq1.at(j - 1);
      to_push.seq2 = '-';
      current.push_back(to_push);
      recursive_print(tab, size, i, j - 1, current);
      current.pop_back();
    }
    else if (tab.table[i][j].top == true) {
      struct pairs to_push;
      to_push.seq1 = '-';
      to_push.seq2 = tab.seq2.at(i - 1);
      current.push_back(to_push);
      recursive_print(tab, size, i - 1, j, current);
      current.pop_back();
    }
    
  }
}