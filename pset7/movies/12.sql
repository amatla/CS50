-- write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Johnny Depp.
-- You may assume that there is only one person in the database with the name Helena Bonham Carter

SELECT title from movies
INNER JOIN stars on movies.id = stars.movie_id
INNER JOIN people on people.id = stars.person_id
WHERE people.name IN ('Johnny Depp', 'Helena Bonham Carter')
GROUP by stars.movie_id
HAVING COUNT(stars.person_id) = 2;
