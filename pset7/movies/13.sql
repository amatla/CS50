-- write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list.

SELECT name from people
JOIN stars on people.id = stars.person_id
JOIN movies on movies.id = stars.movie_id
WHERE people.id = stars.person_id
AND NOT name = 'Kevin Bacon'
AND title IN (SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = 'Kevin Bacon');



