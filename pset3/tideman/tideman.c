#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool is_circle(int, int);
void sort(void); //basic sorting function to sort pairs array
void bubble_sort(void); //bubble sort function for pairs array

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0) //check if "name" is a valid candidate name
        {
            ranks[rank] = i; //upgrade rank with the candidate name index
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0;  i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //for each candidates pair
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            //check if there is a winner in the pair and update the pair count if there is.
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    bubble_sort();
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //for each pair
    for (int i = 0; i < pair_count; i++)
    {
        //lock the pair and check if it created a circle
        locked[pairs[i].winner][pairs[i].loser] = true;
        if (is_circle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = false; //remove the lock if a circle was created
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int locks = 0;

    //iterate over every candidate column
    //if the candidate has no locks, they are a source of the graph
    //and we print their name(s).
    for (int i = 0; i < candidate_count; i++)
    {
        locks = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                locks++;
            }
        }
        if (locks == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

//determine if a circle is created adding the next pair.
bool is_circle(int winner, int loser)
{
    //base condition to end the recursion if we find a circle.
    if (loser == winner)
    {
        return true;
    }

    //check on the loser row to see if locked paths exists
    //if they do explore them and check if they form a circle
    //by calling is_circle() with the index of the locked path.
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (is_circle(winner, i))
            {
                return true;
            }
        }
    }
    //if no circle is found return false.
    return false;
}

void sort(void)
{
    pair tmp;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[i].winner][pairs[i].loser])
            {
                tmp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = tmp;
            }
        }
    }
}

void bubble_sort(void)
{
    int swap = 1;
    while (swap != 0)
    {
        swap = 0;
        for(int i = 0; i < pair_count; i++)
        {
            int j = i + 1;
            if(preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[i].winner][pairs[i].loser])
            {
                pair tmp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = tmp;
                swap++;
            }
        }
    }
}
