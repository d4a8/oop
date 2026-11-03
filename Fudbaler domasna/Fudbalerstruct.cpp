#include <iostream>
using namespace std;

struct date_of_birth
{
    int day;
    char month[10];
    int year;
};

struct Player
{
    char name [20];
    char surname[30];
    char club[30];
    date_of_birth date;
};

void strcopy (char *a, char *b)
{
    while (*b)
    {
        *a = *b;
        a++; b++;
    }
    *a = '\0';
}

int main()
{
    Player player;
    strcopy(player.name, "Zlatan");
    strcopy(player.surname, "Ibrahimovic");
    strcopy(player.club, "Manchester United");
    player.date.day = 3;
    strcopy(player.date.month, "October");
    player.date.year = 1981;
   
    cout << player.name << " " << player.surname << "\n";
    cout << player.club << "\n";
    cout << player.date.day << " " << player.date.month << " " << player.date.year;
   
    return 0;
}