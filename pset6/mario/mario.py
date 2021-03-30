from cs50 import get_int

# ask user for pyramid height
# it should be a valid a integer between 1 and 8
while True:
    height = get_int("Height: ")
    if height in range(1, 9):
        break

# for each line
for i in range(1, height + 1):
    # print spaces
    print((height - i) * " ", end="")
    # print left side hashes
    print(i * "#", end="")
    # print double space in the middle
    print("  ", end="")
    # print right side hashes
    print(i * "#")