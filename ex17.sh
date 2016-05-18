#! /bin/sh

# Test ex17

set -e

green=`tput setaf 2`
reset=`tput sgr0`

echo "${green}Creating database${reset}"
./bin/ex17 db.dat c 20 32

echo "${green}Saving records to db${reset}"
./bin/ex17 db.dat s 1 neil neil@neil.com
./bin/ex17 db.dat s 2 user2 user2@users.com

echo "${green}List all records${reset}"
./bin/ex17 db.dat l

echo "${green}${green}Delete records by id 2${reset}"
./bin/ex17 db.dat d 2

echo "${green}List all records${reset}"
./bin/ex17 db.dat l

echo "${green}Fetch user 1${reset}"
./bin/ex17 db.dat g 1

echo "${green}Saving records to db${reset}"
./bin/ex17 db.dat s 3 user3 user3@users.com

echo "${green}List all records${reset}"
./bin/ex17 db.dat l

echo "${green}Resize db rows${reset}"
./bin/ex17 db.dat r 2 32

echo "${green}List all records${reset}"
./bin/ex17 db.dat l

echo "${green}Resize field size${reset}"
./bin/ex17 db.dat r 2 4

echo "${green}List all records${reset}"
./bin/ex17 db.dat l
