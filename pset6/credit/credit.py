from cs50 import get_string

# get card number from user
while True:
    cc_number = get_string("Number: ")
    if int(cc_number) > 0:
        break

# number of digits in card number
digits = len(cc_number)
isOther = False
checksum = 0

# first two digits of card number
start = int(cc_number[:2])

# calculate lhun checksum number
for i in range(1, digits+1):
    if isOther:
        value = int(cc_number[-i]) * 2
        checksum += value if value < 9 else value - 9
    else:
        checksum += int(cc_number[-i])
    isOther = not isOther

# check id the checksum is valid
isValid = True if (checksum % 10) == 0 else False
if not isValid:
    print("INVALID")

# check if it is a valid credit card
elif isValid and digits == 15 and (start == 34 or start == 37):
    print("AMEX")
elif isValid and digits == 16 and (start in range(51, 56)):
    print("MASTERCARD")
elif isValid and (digits == 13 or digits == 16) and (int(start / 10) == 4):
    print("VISA")