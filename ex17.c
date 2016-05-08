// stdio contains FILE, also knows as a file handle.
// FILE is an opaque type containing information about
// a file or text stream needed to perform input or ouput operations
//
// stdio includes perror
// stdio includes fread, fopen, fclose, and rewind
#include <stdio.h>
#include <assert.h>
// stdlib contains exit function, which causes normal process
// termination
#include <stdlib.h>
// errno contains errno, the number of the last error
#include <errno.h>
// string contains strncpy
#include <string.h>

// Creates constant settings with the C Pre-Processor
#define MAX_DATA 512

// Struct is a fixed size. It is the size of the two
// ints plus the size of the two fixed length arrays
struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    int max_rows;
    struct Address *rows;
};

// Contains a pointer to a file and a pointer to a
// Database struct
struct Connection {
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn);

// Kills the program. When an error is returned from a
// function, it will usually set an "external" variable
// called errno to say what exactly the error was. The
// error is just a number, so perror is used to print
// the error message.
void die(const char *message, struct Connection *conn)
{
    // the if statement evaluates the expression perror(message)
    // if errno is true (non zero)
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    Database_close(conn);
    exit(1);
}

// Pretty prints an Address, given a pointer to that address
void Address_print(struct Address *addr)
{
    printf("%d %s %s\n",
            addr->id, addr->name, addr->email);
}

// Tries to read one object of size DB from the stream
// pointed to by the file variable of the struct that
// the given pointer conn points to, and store that
// object at the location given by the db variable
// of the struct that the given pointer conn points to.
void Database_load(struct Connection *conn)
{
    // Load the number of rows the the DB
    int size_rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);

    if(size_rc != 1) {
	die("Failed to load db size.", conn);
    }

    int max_rows = conn->db->max_rows;

    // Load the rows from the DB
    conn->db->rows = malloc(sizeof(struct Address) * max_rows);
    if(!conn->db->rows) {
	printf("Could not allocate memory for rows.\n");
    }

    int array_rc = fread(conn->db->rows,
			 sizeof(struct Address),
			 max_rows,
			 conn->file);

    if (array_rc != max_rows) {
	die("Failed to load db contents.", conn);
    }
}

// Opens and connects to the database
struct Connection *Database_open(const char *filename, char mode, int max_rows)
{
    // Allocates memory for the Connection  on the heap
    struct Connection *conn = malloc(sizeof(struct Connection));

    // NULL is 0 so boolean works. If malloc failed, then
    // *conn is NULL. !NULL will evaluate to true
    if(!conn) die("Memory error", conn);

    // Initialize the db variable to a pointer that points to
    // memory allocated to store the DB
    conn->db = malloc(sizeof(struct Database));

    // If malloc failed, then the pointer will be NULL
    if(!conn->db) die("Memory error", conn);

    // If the mode is 'c', create the database file or
    // clear the database file. Otherwise, open the database
    // file for reading and writing and load the database.
    if(mode == 'c') {
        conn->file = fopen(filename, "w");

	// Write the number of rows to the DB
	int rc = fwrite(&max_rows, sizeof(int), 1, conn->file);
	if(rc != 1) {
	    die("Could not write count to new DB.", conn);
	}
	
	conn->db->max_rows = max_rows;

	// Allocate memory for the rows
	struct Address rows[max_rows];
	conn->db->rows = malloc(sizeof(struct Address) * max_rows);
	if (!conn->db->rows) {
	    die("Could not allocate memory for rows", conn);
	}

    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    // If opening the file failed, then fopen returns NULL
    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

// Closes the connection to the database by dissociating
// the FILE stream from its underlying file and returning
// the memory allocated by the connection and DB to the OS
void Database_close(struct Connection *conn)
{
    // If conn is not NULL, dissociate the FILE struct from
    // underlying file, free the memory allocated for the DB,
    // and free the memory allocated for the connection
    if(conn) {
	// int
	// fclose(FILE *stream)
        if(conn->file) fclose(conn->file);

	if(conn->db) {
	    if(conn->db->rows) {
		free(conn->db->rows);
	    }

	    free(conn->db);
	}
        
        free(conn);
    }
}

// Write to the DB by:
//    - setting the file position for the DB file to the
//      beginning of the file
//    - Write the DB pointed to the connection to the FILE
void Database_write(struct Connection *conn)
{
    // Sets the file position to the beginning of the file
    rewind(conn->file);

    // Writes the first DB in memory to the DB file from the
    // pointer contained in conn
    int max_rows_rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if(max_rows_rc != 1) {
	die("Failed to write size to database.", conn);
    }

    int rows_rc = fwrite(
	conn->db->rows,
	sizeof(struct Address),
	conn->db->max_rows,
	conn->file);

    // If fwrite did not write one object, kill the process
    if(rows_rc != conn->db->max_rows){
	die("Failed to write database.", conn);
    }

    int rc = fflush(conn->file);
    // If fflush does not return 0, then it did not succesfully complete
    if(rc == -1) die("Cannot flush database.", conn);
}

// Creates the empty database by initializing a
// prototype Address for each row of the DB
void Database_create(struct Connection *conn)
{
    int i = 0;

    for(i = 0; i < conn->db->max_rows; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(
    struct Connection *conn,
    int id,
    const char *name,
    const char *email)
{
    // Creates a pointer addr that points to
    // the address of (the id'th element of the rows variable in the
    // db that
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    // Sets `set` variable
    addr->set = 1;
    char *res = strncpy(addr->name, name, MAX_DATA);
    addr->name[MAX_DATA - 1] = '\0';
    if(!res) die("Name copy failed", conn);

    res = strncpy(addr->email, email, MAX_DATA);
    addr->email[MAX_DATA - 1] = '\0';
    if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    // Gets the address of the id'th element of the DB
    // pointed to by conn
    struct Address *addr = &conn->db->rows[id];

    // If the set variable of the Address is non zero, print the
    // address. If it is zero,  kill the process
    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    // Construct an unset address
    struct Address addr = {.id = id, .set = 0};
    // Update the id'th element of the DB pointed to by
    // conn with the unset address
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;
    int max_rows = db->max_rows;
    for(i = 0; i < max_rows; i++) {
	// Get the pointer to the ith element of the DB, which is an
	// array of Addresses
        struct Address *cur = &db->rows[i];

	// the address's set field is non zero, print the address
        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];
    int id = 0;
    struct Connection *conn = NULL;
    
    // atoi converts a string and converts it to an int
    if(argc > 3) id = atoi(argv[3]);
//    if(id >= MAX_ROWS) die("There's not that many records.", conn);

    switch(action) {
        case 'c':
	    if(argc != 4) {
		die("Need a size to create DB with", NULL);
	    }
	    conn = Database_open(filename, action, atoi(argv[3]));
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", NULL);
            conn = Database_open(filename, action, 0);
            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", NULL);
            conn = Database_open(filename, action, 0);
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete", NULL);
            conn = Database_open(filename, action, 0);
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            conn = Database_open(filename, action, 0);
            Database_list(conn);
            break;
	    
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list",
		NULL);
    }

    Database_close(conn);

    return 0;
}
