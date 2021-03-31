from cs50 import get_string


def main():
    # get text to grade from user
    text = get_string("Text: ")
    results = get_results(text)
    grade = get_grade(**results)
    print_grade(grade)


def get_results(text):
    """ retrurns a dictionary containing the number of letters, words and sentences in a text."""
    results = {'letters': 0, 'words': 0, 'sentences': 0}
    for i in text:
        if i.isalpha():
            results['letters'] += 1
        elif i.isspace():
            results['words'] += 1
        elif (i == '.' or i == '?' or i == '!'):
            results['sentences'] += 1
    #account for the last word of the text that doesnt end with a space.
    results['words'] += 1
    return results


def get_grade(letters, words, sentences):
    """Calculates the Coleman-Liau index, based on the number of letters words and sentences in a text. Rounds the result to the neares integer"""
    l = (letters * 100) / words
    s = (sentences * 100) / words
    index = (0.0588 * l) - (0.296 * s) - 15.8
    return round(index)


def print_grade(index):
    # print grade based on the Coleman-Liau index
    if index < 0:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()