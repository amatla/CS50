import sys
import csv


def main():
    # verify user input
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # people is a list of dictionaries
    # each dictionary has all the informations for one person
    people = []

    # STRs is a dictionary containing the STRs (Short Tandem Repeats) in the database
    STRs = {}

    # open database file and populate the people dictionary
    with open(sys.argv[1]) as database:
        reader = csv.DictReader(database)
        for row in reader:
            people.append(row)
        # populate the STRs dictionary with the strs from the first row of the database
        # (we skip the first entry which us the 'name' field)
        for key in (reader.fieldnames[1:]):
            STRs[key] = 0

    # open the sequence file and store the dna in a string
    with open(sys.argv[2]) as f:
        dna = f.readline()

    # for each STR in STRs count the max number of consecutive occurences in the dna file
    for key in STRs.keys():
        STRs[key] = count_sub(dna, key)

    # check for a match in the poeple dictionary
    # print the person name if we find one or "No match" otherwise
    print(f"{find_match(people, STRs)}")


def count_sub(dna, sub):
    """Count the max number of consecutive occurrences of the substring sub in the string dna"""
    sub_len = len(sub)
    dna_len = len(dna)
    count = 0

    # iterate over each char of the dna string
    for start in range(dna_len):
        # if we find a match for our substring - reset the current counter
        if dna[start:start+sub_len] == sub:
            tmp_count = 0
            # count how many consecutive occurrences we find
            while dna[start:start+sub_len] == sub:
                tmp_count += 1
                start += sub_len
            # update counter if we find a bigger number of consecutive occurrences
            if tmp_count > count:
                count = tmp_count
    # return the max number of consecutive occurrences
    return count


def compare_str(person, STRs):
    """ Compare a person STRs with the STRs stored in the dna database """
    for key in STRs.keys():
        if (STRs[key] != (int(person[key]))):
            return False
    return True


def find_match(people, STRs):
    """ Comapre each person in the people dictionary with the STRs in the database.
        Return the name of the match if we find one - 'No match' otherwise """
    for person in people:
        if compare_str(person, STRs):
            return person["name"]
    return "No match"


if __name__ == "__main__":
    main()