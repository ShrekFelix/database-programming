#include <fstream>
#include <pqxx/pqxx>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int main (int argc, char *argv[]) { 
  //Allocate & initialize a Postgres connection object
  connection *C;
  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }
  /* Create a transactional object. */
  work W(*C);
  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  /* Create SQL statement */
  string sql = " \
    DROP TABLE IF EXISTS STATE;				\
    CREATE TABLE STATE(					\
      STATE_ID INT PRIMARY KEY NOT NULL,		\
      NAME TEXT NOT NULL);				\
    DROP TABLE IF EXISTS COLOR;				\
    CREATE TABLE COLOR(					\
      COLOR_ID INT PRIMARY KEY NOT NULL,		\
      NAME TEXT NOT NULL);				\
    DROP TABLE IF EXISTS TEAM;				\
    CREATE TABLE TEAM(					\
      TEAM_ID INT PRIMARY KEY NOT NULL,			\
      NAME TEXT NOT NULL,				\
      STATE_ID INT REFERENCES STATE(STATE_ID) NOT NULL, \
      COLOR_ID INT REFERENCES COLOR(COLOR_ID) NOT NULL, \
      LAST_NAME CHAR(50),				\
      WINS INT,						\
      LOSSES INT);					\
    DROP TABLE IF EXISTS PLAYER;			\
    CREATE TABLE PLAYER(				\
      PLAYER_ID INT PRIMARY KEY NOT NULL,		\
      TEAM_ID INT REFERENCES TEAM(TEAM_ID) NOT NULL,	\
      UNIFORM_NUM INT NOT NULL,				\
      FIRST_NAME TEXT,					\
      LAST_NAME TEXT,					\
      MPG REAL,						\
      PPG REAL,						\
      RPG REAL,						\
      APG REAL,						\
      SPG REAL,						\
      BPG REAL);					\
  ";
  W.exec( sql );
  ifstream fs("player.txt");
  string line;
  while(getline(fs, line)){
    replaceAll(line, " ", ",");
    cout << line << '\n';
    sql = "\
      INSERT INTO PLAYER (PLAYER_ID, TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) \
      VALUES (" + line + ")";
  }
  W.exec( sql );
  
  /* Execute SQL query */
  
  W.commit();

  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


