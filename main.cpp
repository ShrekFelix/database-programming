#include <fstream>
#include <pqxx/pqxx>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

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
  
  ifstream fs;
  
  fs.open("state.txt");
  string s1, s2;
  while(fs >> s1 >> s2){
    sql = "\
      INSERT INTO STATE \
      VALUES (" + s1 + ",'" + s2 + "')";
    cout << sql << '\n';
    W.exec( sql );
  }
  fs.close();
  
  fs.open("color.txt");
  string c1,c2;
  while(fs >> c1 >> c2){
    sql = "\
      INSERT INTO COLOR \
      VALUES (" + c1 + ",'" + c2 + "')";
    cout << sql << '\n';
    W.exec( sql );
  }
  fs.close();
  
  fs.open("team.txt");
  string t1,t2,t3,t4,t5,t6;
  while(fs >> t1 >> t2 >> t3 >> t4 >> t5 >> t6){
    sql = "\
      INSERT INTO TEAM \
      VALUES (" + t1 + ",'" + t2 + "'," + t3 + "," + t4 + "," + t5 + "," + t6 + ")";
    cout << sql << '\n';
    W.exec( sql );
  }
  fs.close();
  
  fs.open("player.txt");
  string p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11;
  while(fs >> p1 >> p2 >> p3 >> p4 >> p5 >> p6 >> p7 >> p8 >> p9 >> p10 >> p11){
    sql = "\
      INSERT INTO PLAYER \
      VALUES (" + p1 + "," + p2 + "," + p3 + ",'" + p4 + "','" + p5 + "'," + p6 + "," + p7 + "," + p8 + "," + p9 + "," + p10 + "," + p11 + ")";
    cout << sql << '\n';
    W.exec( sql );
  }
  fs.close();
  /* Execute SQL query */
  
  W.commit();

  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


