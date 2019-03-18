#include "query_funcs.h"

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg){
}
void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses){
}
void add_state(connection *C, string name){
}
void add_color(connection *C, string name){
}
/*
Each query function should print its output. The format of this
output should be as follows. The first row of output should contain 
each field name (separated by a single space character. The next rows
 of output should contain the values returned from the query, 
 each also separated by a single space. The field name and each 
 row of output should appear one line after the next.
*/
/*
show all attributes of each player with average statistics that fall
between the min and max (inclusive) for each enabled statistic
*/
void query1(connection *C,
            int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
        ){
    work W(*C);
    string sql = "SELECT * FROM PLAYER WHERE PLAYER_ID > 0";
    if(use_mpg){
        sql += " AND MPG <=" + to_string(max_mpg) + " AND MPG >=" + to_string(min_mpg);
    }
    if(use_ppg){
        sql += " AND PPG <=" + to_string(max_ppg) + " AND PPG >=" + to_string(min_ppg);
    }
    if(use_rpg){
        sql += " AND RPG <=" + to_string(max_rpg) + " AND RPG >=" + to_string(min_rpg);
    }
    if(use_apg){
        sql += " AND APG <=" + to_string(max_apg) + " AND APG >=" + to_string(min_apg);
    }
    if(use_spg){
        sql += " AND SPG <=" + to_string(max_spg) + " AND SPG >=" + to_string(min_spg);
    }
    if(use_bpg){
        sql += " AND BPG <=" + to_string(max_bpg) + " AND BPG >=" + to_string(min_bpg);
    }
    sql += ";";
    result r = W.exec(sql);
    W.commit();
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG\n";
    for(auto row:r){
        for(auto field:row){
            cout << field.c_str() << " ";
        }
        cout << '\n';
    }

}

/*
show the name of each team with the indicated uniform color
*/
void query2(connection *C, string team_color){
    work W(*C);
    string sql="SELECT T.NAME FROM TEAM AS T, COLOR AS C WHERE T.COLOR_ID=C.COLOR_ID AND C.NAME= " + procStr(team_color) + " ;";
    result r = W.exec(sql);
    W.commit();
    cout << "NAME\n";
    for(auto row:r){
        for(auto field:row){
            cout << field.c_str() << " ";
        }
        cout << '\n';
    }
}

/*
show the first and last name of each player that plays for the indicated
team, ordered from highest to lowest ppg (points per game)
*/
void query3(connection *C, string team_name){
    work W(*C);
    string sql="\
        SELECT P.FIRST_NAME, P.LAST_NAME \
        FROM PLAYER AS P,TEAM AS T \
        WHERE P.TEAM_ID=T.TEAM_ID AND T.NAME= "+procStr(team_name)+\
        " ORDER BY P.PPG DESC;";
    result r = W.exec(sql);
    W.commit();
    cout<<"FIRST_NAME LAST_NAME\n";
    for(auto row:r){
        for(auto field:row){
            cout << field.c_str() << " ";
        }
        cout << '\n';
    }
}

/*
show first name,last name,and jersey number of each player that
plays in the indicated state and wears the indicated uniform color
*/
void query4(connection *C, string team_state, string team_color){
    work W(*C);
    string sql="\
    SELECT P.FIRST_NAME, P.LAST_NAME, P.UNIFORM_NUM \
    FROM PLAYER AS P,STATE AS S,TEAM AS T,COLOR AS C \
    WHERE P.TEAM_ID=T.TEAM_ID AND T.STATE_ID=S.STATE_ID AND S.NAME="+procStr(team_state)+" AND T.COLOR_ID=C.COLOR_ID AND C.NAME="+ procStr(team_color) +";";
    result r = W.exec(sql);
    W.commit();
    cout<<"FIRST_NAME LAST_NAME UNIFORM_NUM\n";
    for(auto row:r){
        for(auto field:row){
            cout << field.c_str() << " ";
        }
        cout << '\n';
    }
}

/*
show first name and last name of each player,and team name and
number of wins for each team that has won more than the indicated number of
games
*/
void query5(connection *C, int num_wins){
    work W(*C);
    string sql="\
    SELECT P.FIRST_NAME, P.LAST_NAME, T.NAME, T.WINS \
    FROM PLAYER AS P,TEAM AS T \
    WHERE P.TEAM_ID=T.TEAM_ID AND T.WINS>"+ to_string(num_wins) + " ;";
    result r = W.exec(sql);
    W.commit();
    cout<<"FIRST_NAME LAST_NAME TEAM_NAME WINS\n";
    for(auto row:r){
        for(auto field:row){
            cout << field.c_str() << " ";
        }
        cout << '\n';
    }
}
