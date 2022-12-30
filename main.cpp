/*
    myMusic SQL
    Application allows user to browse database with artist, albums and songs,
    and add or delete songs from user's list of favorite songs.
*/

#include <iostream>
#include <Windows.h>
#include <mysql.h>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>

using namespace std;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;

//checking if song is on user's list of liked songs
bool checkSong(int song)
{
    std::ostringstream checking;
    checking << "SELECT * FROM liked WHERE song_id = " << song;
    string sql_songcheck = checking.str();

    mysql_query(conn, sql_songcheck.c_str());
    res = mysql_store_result(conn);

    if(row = mysql_fetch_row(res))
    {
        return true;
    } else return false;
}


int main()
{
    //connection
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "password", "mymusic", 0, NULL, 0);

    if(conn)
    {
        //menu control
        int menu_level = 1;
        int menu1, menu2, menu3, menu4;

        do
        {
        if(menu_level == 1)
        {
            system("CLS");

            cout << "=== MENU ===\n";
            cout << "1. Your list\n";
            cout << "2. Browse the catalog\n";
            cout << "0. Quit\n\n";
            cout << "Choose number: ";
            cin >> menu1;

            menu_level = (menu1 > 0) ? 2 : 0; //changing level of menu
        }
            do
            {
            if(menu_level == 2)
            {
                system("CLS");
                if(menu1 == 1) //user's list
                {
                    //making list of song chosen by user
                    mysql_query(conn, "SELECT l.*, a.name artist_name, s.title song_title, s.id, a.id, x.id, x.artist_id FROM liked l, song s, artist a, album x WHERE s.id = l.song_id and s.album_id = x.id and x.artist_id = a.id ORDER BY a.name, s.title");
                    res = mysql_store_result(conn);

                    int counter = 0; //counter to count if there is any songs on the list

                    while(row = mysql_fetch_row(res)) // printing all songs
                    {
                        cout << setw(5) << row[0] << " | " << row[1] << " - " <<  row[2] << endl;
                        counter++;
                    }

                    if(counter != 0) //if list is not empty, enable option to choose song to delete from list or go back
                    {
                        cout << "    0 | GO BACK\n\n";
                        cout << "*Choose song to delete or input 0 to return to menu*\n";
                        cout << "Choice: ";
                        cin >> menu2;

                        if(menu2 == 0) { menu_level = 1; } //changing level of menu
                    }
                    else
                    {
                        //if list is empty go back to main menu
                        cout << "This list is empty! Add some songs.\n";
                        system("pause");
                        menu_level = 1;
                    }

                    //deleting chosen song from the list
                    if(menu2 != 0 && counter != 0)
                    {
                        if(checkSong(menu2))
                            {
                            int qstate;

                            stringstream delete_song;

                            delete_song << "DELETE FROM liked WHERE song_id = " << menu2;

                            string query = delete_song.str();
                            const char * q = query.c_str();
                            qstate = mysql_query(conn, q);

                            if(qstate == 0)
                            {
                                cout << "Song was deleted from your list\n";
                                Sleep(2000);
                            }
                            else
                            {
                                cout << "Error!\n";
                                Sleep(2000);
                            }
                        }
                        else { //if user input a wrong number
                            cout << "This song is not on your list!\n";
                            Sleep(2000);
                        }
                    }
                }

                //collecting and printing list of artist
                if(menu1 == 2)
                {
                    mysql_query(conn, "SELECT * FROM artist ORDER by name");
                    res = mysql_store_result(conn);

                    cout << "================================\n";
                    cout << " id | Artist's name\n";
                    cout << "================================\n";

                    while(row = mysql_fetch_row(res))
                    {
                        cout << setw(3) << row[0] << " | " << row[1] << endl;
                    }

                    //user's input
                    cout << "  0 | Go back\n\n";
                    cout << "Choose artist's id: ";
                    cin >> menu2;

                    menu_level = (menu2 > 0) ? 3 : 1; //changing level of menu
                }

            }

                do
                {
                if(menu_level == 3)
                {
                    system("CLS");

                    //collecting and printing information about artist
                    std::ostringstream artist_info;
                    artist_info << "SELECT * FROM artist WHERE id = " << menu2;
                    string sql_Statement = artist_info.str();

                    mysql_query(conn, sql_Statement.c_str());
                    res = mysql_store_result(conn);

                    while(row = mysql_fetch_row(res))
                    {
                        cout << "ARTIST: " << row[1] << endl;
                        cout << "ORIGIN: " << row[2] << endl;
                        cout << "FORMED: " << row[4] << endl;
                        cout << "ACTIVE: ";

                        string active;

                        active = row[3];

                        if(active == "1")
                        {
                            cout << "Yes \n\n";
                        } else cout << "No \n\n";
                    }

                    //collecting and printing list of studio albums released by chosen artist
                    std::ostringstream buf;
                    buf << "SELECT * FROM album WHERE artist_id = " << menu2 << " ORDER BY album_order";
                    string str = buf.str();

                    mysql_query(conn, str.c_str());
                    res = mysql_store_result(conn);

                    cout << "================================\n";
                    cout << " id | Album's title\n";
                    cout << "================================\n";

                    while(row = mysql_fetch_row(res))
                    {
                        cout << setw(3) << row[0] << " | " << row[3] << endl;
                    }

                    //user's input
                    cout << "  0 | Go back\n\n";
                    cout << "Choose album's id: ";
                    cin >> menu3;

                    menu_level = (menu3 > 0) ? 4 : 2; //changing level of menu
                }

                    do
                    {
                    if(menu_level == 4)
                    {
                    system("CLS");

                        //collecting and printing information about chosen album
                        std::ostringstream album_info;
                        album_info << "SELECT a.*, x.name art_name, DATE_FORMAT(a.released, '%d.%m.%Y') date FROM album a, artist x WHERE a.id = " << menu3 << " and x.id = a.artist_id";
                        string sql_Statement = album_info.str();

                        mysql_query(conn, sql_Statement.c_str());
                        res = mysql_store_result(conn);

                        while(row = mysql_fetch_row(res))
                        {
                            cout << "  ARTIST: " << row[6] << endl;
                            cout << "   TITLE: " << row[3] << endl;
                            cout << "RELEASED: " << row[7] << endl;
                            cout << "   LABEL: " << row[5] << endl;
                        }
                        cout << "\nTRACKLIST\n";
                        cout << "=======================================\n";
                        cout << "  id | Order on album and song title\n";
                        cout << "=======================================\n";

                        //collecting and printing list of songs from chosen album
                        std::ostringstream tracklist;
                        tracklist << "SELECT *, TIME_FORMAT(time, '%i:%S') lenght FROM song WHERE album_id = " << menu3 << " ORDER BY song_order";
                        string sql_tracklist = tracklist.str();

                        mysql_query(conn, sql_tracklist.c_str());
                        res = mysql_store_result(conn);

                        while(row = mysql_fetch_row(res))
                        {
                            cout << setw(4) << row[0] << " | " << setw(2) << row[2] << ". " << row[3] << " (" << row[5] << ")" << endl;
                        }

                        //user's input
                        cout << "   0 | Go Back\n\n";
                        cout << "Choose song's id: ";
                        cin >> menu4;

                        menu_level = (menu4 > 0) ? 4 : 3; //changing level of menu

                        if(menu4 != 0)
                        {
                            //checking if chosen song is already on the list, if not, adding it to the list
                            if(!checkSong(menu4))
                            {
                                int qstate;

                                stringstream add_song;

                                add_song << "INSERT INTO liked(song_id) VALUES (" << menu4 << ")";

                                string query = add_song.str();
                                const char * q = query.c_str();
                                qstate = mysql_query(conn, q);

                                if(qstate == 0)
                                {
                                    cout << "Song added to your list successfully!\n";
                                    Sleep(2000);
                                }
                                else
                                {
                                    cout << "Error, something went wrong!\n";
                                    Sleep(2000);
                                }

                            }
                            else {
                                cout << "This song is already on your list!\n";
                                Sleep(2000);
                                }
                        }
                    }

                    } while(menu_level == 4);
                } while(menu_level == 3);
            }while(menu_level == 2);
        } while (menu_level == 1);

    }
    else //if connection with database is not established
    {
        cout << "Error! No Connection! Something is wrong!\n";
        cout << "Check SQL connection settings and try again!\n";
    }

    return 0;
}
