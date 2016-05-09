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

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_rows;
    int max_data;
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
	die("Failed to load db size.\n", conn);
    }

    int max_rows = conn->db->max_rows;

    int max_data_rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);

    if(max_data_rc != 1) {
	die("Could not load size of data\n.", conn);
    }

    // Load the rows from the DB
    conn->db->rows = malloc(sizeof(struct Address) * max_rows);
    if(!conn->db->rows) {
	printf("Could not allocate memory for rows.\n");
    }

    int i = 0;
    for(i = 0; i < max_rows; i++) {

        // Read the id
	int id_rc = fread(&conn->db->rows[i].id, sizeof(int), 1, conn->file);
	if (id_rc != 1) {
	    die("Could not load id.\n", conn);
	}

	// Read the set field
	int set_rc = fread(&conn->db->rows[i].set, sizeof(int), 1, conn->file);
	if (set_rc != 1) {
	    die("Could not load set.\n", conn);
	}

	// Allocate memory for the name
	conn->db->rows[i].name = malloc(sizeof(char) * conn->db->max_data);
	if(!conn->db->rows[i].name) {
	    die("Could not allocate memory for name", conn);
	}

	// Read the name
	int name_rc = fread(conn->db->rows[i].name,
			    sizeof(char),
			    conn->db->max_data,
			    conn->file);
	if(name_rc != conn->db->max_data) {
	    die("Could not load name.\n", conn);
	}

	// Allocate memory for the email
	conn->db->rows[i].email = malloc(sizeof(char) * conn->db->max_data);
	if(!conn->db->rows[i].email) {
	    die("Could not allocate memory for email", conn);
	}

	// Read the email
	int email_rc = fread(conn->db->rows[i].email,
			    sizeof(char),
			    conn->db->max_data,
			    conn->file);
	if(email_rc != conn->db->max_data) {
	    die("Could not load email.\n", conn);
	}
    }
}

// Opens and connects to the database
struct Connection *Database_open(const char *filename,
				 char mode,
				 int max_rows,
				 int max_data)
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

	// Write the max data size to the DB
	int max_data_rc = fwrite(&max_data, sizeof(int), 1, conn->file);
	if(max_data_rc != 1) {
	    die("Could not write max data to new DB.", conn);
	}

	conn->db->max_data = max_data;

	// Allocate memory for the rows
	conn->db->rows = malloc(sizeof(struct Address) * max_rows);
	if (!conn->db->rows) {
	    die("Could not allocate memory for rows", conn);
	}

	// Initialize arrays for the rows
	int i = 0;
	for(i = 0; i < max_rows; i++) {
	    conn->db->rows[i].name = malloc(sizeof(char) * max_data);
	    if (!conn->db->rows[i].name) {
		die("Could not allocate memory for name", conn);
	    }

	    conn->db->rows[i].email = malloc(sizeof(char) * max_data);
	    if(!conn->db->rows[i].email) {
		die("Could not allocate memory for email", conn);
	    }
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
		int i = 0;
		for (i = 0; i < conn->db->max_rows; i++)
		{
		    free(conn->db->rows[i].name);
		    free(conn->db->rows[i].email);
		}
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

    int max_rows_rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if(max_rows_rc != 1) {
	die("Failed to write size to database.", conn);
    }

    int max_data_rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
    if(max_data_rc != 1) {
	die("Failed to write max data to database.", conn);
    }

    int i = 0;
    for(i = 0; i < conn->db->max_rows; i++)
    {
	// Write id to file
	int id_rc = fwrite(&conn->db->rows[i].id,
			   sizeof(int),
			   1,
			   conn->file);
	if(id_rc != 1) {
	    die("Could not write id to file", conn);
	}

	// Write set to file
	int set_rc = fwrite(&conn->db->rows[i].set,
			    sizeof(int),
			    1,
			    conn->file);
	if(set_rc != 1) {
	    die("Could not write set to file", conn);
	}

	// Write name to file
	int name_rc = fwrite(conn->db->rows[i].name,
			     sizeof(char),
			     conn->db->max_data,
			     conn->file);
	if(name_rc != conn->db->max_data) {
	    die("Could not write name to file", conn);
	}

	// Write email to file
	int email_rc = fwrite(conn->db->rows[i].email,
			      sizeof(char),
			      conn->db->max_data,
			      conn->file);
	if(email_rc != conn->db->max_data) {
	    die("Could not write email to file", conn);
	}

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
        struct Address addr = {.id = i, .set = 0, .name = conn->db->rows[i].name, .email = conn->db->rows[i].email};
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

    int max_data = conn->db->max_data;
    // Sets `set` variable
    addr->set = 1;
    char *res = strncpy(addr->name, name, max_data);
    addr->name[max_data - 1] = '\0';
    if(!res) die("Name copy failed", conn);
    res = strncpy(addr->email, email, max_data);
    addr->email[max_data - 1] = '\0';
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

void Database_resize(struct Connection *conn, int new_size)
{
    // Write new size to file

    rewind(conn->file);

    int rc = fwrite(&new_size, sizeof(int), 1, conn->file);

    if(rc != 1) {
	die("Could not write new size to file", conn);
    }

    // Write rows to file

    int i = 0;
    struct Address resized_rows[new_size];

    for(i = 0; i < new_size; i++) {
	if(i < conn->db->max_rows) {
	    resized_rows[i] = conn->db->rows[i];
	} else {
	    struct Address addr = {.id = i, .set = 0};
	    resized_rows[i] = addr;
	}
    }

    rc = fwrite(resized_rows, sizeof(struct Address), new_size, conn->file);

    if(rc != new_size) {
	die("Could not write reized array to file", conn);
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
	    if(argc != 5) {
		die("Need a row number and data size to create DB with", NULL);
	    }
	    conn = Database_open(filename, action, atoi(argv[3]), atoi(argv[4]));
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", NULL);
            conn = Database_open(filename, action, 0, 0);
            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", NULL);
            conn = Database_open(filename, action, 0, 0);
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete", NULL);
            conn = Database_open(filename, action, 0, 0);
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            conn = Database_open(filename, action, 0, 0);
            Database_list(conn);
            break;
	    
	case 'r':
	    if(argc != 4) die("Need size to resize with", NULL);
	    conn = Database_open(filename, action, 0, 0);
	    Database_resize(conn, atoi(argv[3]));
	    break;

        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, r=resize",
		NULL);
    }

    Database_close(conn);

    return 0;
}
