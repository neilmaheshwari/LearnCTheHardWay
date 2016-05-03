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
#include <string.h>

// Creates constant settings with the C Pre-Processor
#define MAX_DATA 512
#define MAX_ROWS 100

// Struct is a fixed size. It is the size of the two
// ints plus the size of the two fixed length arrays
struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

// Struct is a fixed size because it is a fixed length
// array of Address structs
struct Database {
    struct Address rows[MAX_ROWS];
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
    // from the man page:
    // size_t
    // fread(void * restrict ptr, size_t size, size_t, nmemb,
    //     FILE * restrict stream);
    //
    // Function fread reads nmemb objects, each size bytes long, 
    // from the stream pointed to by stream, storing them at the
    // location given by ptr
    //
    // In this context, the restrict keyword is a declaration
    // of intent given by the programmer to the compiler. It
    // says that for the lifetime of the pointer, only the pointer
    // itself or a value dervied from it will be used to access
    // theo bject to which it points. This can aid with optimizations.
    // More information: https://en.wikipedia.org/wiki/Restrict
    //
    // fread returns the number of objects read. It also advances
    // the poisition indicator for the stream by the number of
    // bytes read.
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to load database.", conn);
}

// Opens and connects to the database
struct Connection *Database_open(const char *filename, char mode)
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
	// fopen(const char *restrict filename, const char *restrict mode);
	// fopen opens the file whose name is the string pointed
	// to by _filename_ and associate a stream mode. The argument
	// mode points to a string beginning with a character that is
	// from the allowed set (see the man page). In our case:
	// ``r+`` -> Opens for reading and writing. The stream is
	//           positioned at the beginning of the file
	// ``w``  -> Truncate to zero length or create text file for
	//           writing. The stream is positioned at the beginning
	//           of the file.
        conn->file = fopen(filename, "w");
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
        if(conn->db) free(conn->db);
        free(conn);
    }
}

// Write to the DB by:
//    - setting the file position for the DB file to the
//      beginning of the file
//    - Write the DB pointed to the connection to the FILE
void Database_write(struct Connection *conn)
{
    // void
    // rewind(FILE *stream);
    //
    // The rewind() function sets the file position indicator for
    // the stream pointed to by stream to the beginning of the file.

    // Sets the file position to the beginning of the file
    rewind(conn->file);

    // size_t
    // fwrite(const void *restrict ptr, size_t size,
    //           size_t nitems, FILE *restrict stream);
    // The function fwrite() writes nitems objects, each size
    // bytes long, to the stream pointed to by stream, obtaining
    // them from the location given by ptr.
    //
    // The functions fwrite() advances the file position indicator
    // for the file position indicator for the stream by the number
    // of bytes read or written. The return value is the number of
    // objects read or written. If an error occurs, or the
    // end-of-file is reached, the return value is a short object
    // count (or zero)
    //
    // Writes the first DB in memory to the DB file from the
    // pointer contained in conn
    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);

    // If fwrite did not write one object, kill the process
    if(rc != 1) die("Failed to write database.", conn);

    // fflush flushes a stream. When the stream is open for output,
    // fflush writes to the file the contents of the buffer associated
    // with the stream
    //
    // We need this because the OS determines the optimal time to write
    // the data automatically to disk. So, fflush "commits-to-disk"
    // and lets you make sure that the data is written directly to
    // disk, instaed of the operating-system buffers.
    rc = fflush(conn->file);
    // If fflush does not return 0, then it did not succesfully complete
    if(rc == -1) die("Cannot flush database.", conn);
}

// Creates the empty database by initializing a
// prototype Address for each row of the DB
void Database_create(struct Connection *conn)
{
    int i = 0;

    for(i = 0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    // Creates a pointer addr that points to
    // the address of (the id'th element of the rows variable in the
    // db that
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    // Sets `set` variable
    addr->set = 1;
    // WARNING: bug, read the "How To Break It" and fix this
    char *res = strncpy(addr->name, name, MAX_DATA);
    // demonstrate the strncpy bug
    // strncpy returns the destination buffer
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

    for(i = 0; i < MAX_ROWS; i++) {
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
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    // atoi converts a string and converts it to an int
    if(argc > 3) id = atoi(argv[3]);
    if(id >= MAX_ROWS) die("There's not that many records.", conn);

    switch(action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", conn);

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}
