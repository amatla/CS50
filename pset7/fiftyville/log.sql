-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2020 and that it took place on Chamberlin Street.

-- get information from the crime scene - time of theft was 10:15am - three witnesses interrogated that day
SELECT description FROM crime_scene_reports
WHERE year = 2020
AND month = 7
AND day = 28
AND street = 'Chamberlin Street';

-- get transcript for the three interviews --
-- Ruth -- theft drove away whitin 10 minutes of theft --> check parking lot camera
-- Eugene -- recognized the thief saw him in the morning at the ATM on Fifer street withdrawing money --> check atm transactions
-- Raymond  -- thief talked on the phone with accomplice and asked them to buy tickets for first flight out of fiftyville next day
            --> check phone calls -- airport -- flights
SELECT name, transcript from interviews
WHERE transcript LIKE "%courthouse%"
AND year = 2020
AND month = 7
AND day = 28;

-- get licence plates of people exiting courthouse in whitin 10 minutes of the crime being committed form parking lot cameras
SELECT license_plate FROM courthouse_security_logs
WHERE activity = 'exit'
AND year = 2020
AND month = 7
AND day = 28
AND hour = 10
AND minute > 15
AND minute < 25;

-- atm account numbers for transaction from fifer street the day of the crime
SELECT account_number FROM atm_transactions
WHERE year = 2020
AND month = 7
AND day = 28
AND atm_location = 'Fifer Street';

-- get list of poeple id from bank account with accounts number that did a transaction from the atm
SELECT person_id FROM bank_accounts
WHERE account_number IN (
SELECT account_number FROM atm_transactions
WHERE year = 2020
AND month = 7
AND day = 28
AND atm_location = 'Fifer Street');

-- phone calls less than 60 seconds long the day of the crimw --
SELECT caller receiver FROM phone_calls
WHERE year = 2020
AND month = 7
AND day = 28
and duration < 60;

-- first flight the day after the crime leaving from Fiftyville --
SELECT id FROM flights
WHERE year = 2020
AND month = 7
AND day = 29
AND origin_airport_id IN (SELECT id FROM airports WHERE city = 'Fiftyville')
ORDER by hour, minute DESC
LIMIT 1;

-- list of passport numbers on that flight
SELECT passport_number FROM passengers
WHERE flight_id IN (SELECT id FROM flights
WHERE year = 2020
AND month = 7
AND day = 29
AND origin_airport_id IN (SELECT id FROM airports WHERE city = 'Fiftyville')
ORDER by hour, minute DESC
LIMIT 1);

-- we can combine all of the above to find the thief --> Ernest
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020
AND month = 7
AND day = 28
AND hour = 10
AND minute > 15
AND minute < 25)
AND passport_number IN (SELECT passport_number FROM passengers
WHERE flight_id IN (SELECT id FROM flights
WHERE year = 2020
AND month = 7
AND day = 29
AND origin_airport_id IN (SELECT id FROM airports WHERE city = 'Fiftyville')
ORDER by hour, minute DESC
LIMIT 1))
AND phone_number IN (SELECT caller FROM phone_calls
WHERE year = 2020
AND month = 7
AND day = 28
AND duration < 60)
AND person_id IN (SELECT person_id FROM bank_accounts
WHERE account_number IN (SELECT account_number FROM atm_transactions
WHERE year = 2020
AND month = 7
AND day = 28
AND atm_location = 'Fifer Street'));

-- use the thief name to find the accomplice --> Berthold
SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE year = 2020
AND month = 7
AND day = 28
and duration < 60
and caller = (
SELECT phone_number FROM people
WHERE name = 'Ernest');

-- get destination airport for thief flight --> London
SELECT city FROM airports
where id = (SELECT destination_airport_id FROM flights
WHERE flights.id = (SELECT id FROM flights
WHERE year = 2020
AND month = 7
AND day = 29
AND origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
ORDER by hour, minute DESC
LIMIT 1));