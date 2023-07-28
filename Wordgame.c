#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct player
{
    char playerName[50];
    int level;
};

int switchLanes(char *text);
int verifyplayer(char *playerName);
void storeplayer(void);
int checkplayerID(char *playerName);
int getPreviousCheckPoint(char *userName);

char currentplayer[50];

int main()
{
    printf("Welcome to Episode! Please enter your name: ");
    fflush(stdin);

    storeplayer();
    fflush(stdin);
    int episode_number = getPreviousCheckPoint(currentplayer);
    if (episode_number == 1)
    {
        printf("\nWelcome %s Continuing at episode %d \n", currentplayer, episode_number);
    }

    char choice;
   
    while (episode_number <= 5)
    {
        printf("\n--- EPISODE %d ---\n", episode_number);

        if (episode_number == 1)
        {
            choice = switchLanes("You are a student at a high school. It's the first day of school and you're running late. What do you do?\n A. Skip breakfast and run to school\nB. Take your time and go to school late\nC. Ask your parents for a ride\n D. Exit game. \n Enter your choice (A/B/C/D): ");
            if (choice == 'A' || choice == 'a')
            {
                printf("\nYou run to school and make it just in time. You're feeling energized and ready for the day!");
            }
            else if (choice == 'B' || choice == 'b')
            {
                printf("\nYou take your time and go to school late. You miss your first class and start the day off on the wrong foot.");
            }
            else if (choice == 'C' || choice == 'C')
            {
                printf("\nYour parents give you a ride to school. You make it on time, but feel embarrassed.");
            }
            else if (choice == 'D' || choice == 'd')
            {
                //exit_game();
                printf("\nExiting game...\n");
                return 0;
            }
            else
            {
                printf("\nInvalid choice! Please try again.");
                continue;
            }
            episode_number++;
            //savecheckpoint(currentplayer, episode_number);
        }
    }
    return 0;
}

int switchLanes(char *text)
{
    int i = 0;
    while (text[i] != '\0')
    {
        usleep(8000);

        printf("%c", text[i]);
        i++;
    }
    char choice;
    fflush(stdin);
    scanf(" %c", &choice);
    return choice;
}

void storeplayer()
{
    FILE *fp;
    struct player Newplayer;
    fp = fopen("playerData.txt", "a+");
    // printf("Okay here");
    scanf(" %s", Newplayer.playerName);
    strcpy(currentplayer, Newplayer.playerName);
    if (verifyplayer(Newplayer.playerName))
    {
        int playerID = checkplayerID(Newplayer.playerName);
        printf("Welcome back, %s!\n", Newplayer.playerName);
        printf("Your previous level was %d.\n", getPreviousCheckPoint(Newplayer.playerName));
        printf("Do you want to start a new game? (y/n) ");
        char answer;
        scanf(" %c", &answer);
        if (answer == 'y' || answer == 'Y')
        {
            Newplayer.level = 1;
            printf("Creating a new game for %s.\n", Newplayer.playerName);
            int struct_size = sizeof(struct player);
            int struct_index = playerID - 1; // index of the struct, starting from 0
            int offset = struct_index * struct_size;
            fseek(fp, offset, SEEK_SET);
            fwrite(&Newplayer, sizeof(struct player), 1, fp);
        }
        else
        {
            printf("Continuing your previous game...\n");
        }
    }
    else
    {
        Newplayer.level = 1;
        printf("Creating a new player %s.\n", Newplayer.playerName);
        fwrite(&Newplayer, sizeof(struct player), 1, fp);
    }
    fclose(fp);
}

int verifyplayer(char *playerName)
{
    FILE *fp;
    fp = fopen("playerData.txt", "r");
    struct player currplayer;

    if (fp == NULL)
    {
        printf("Encountered an error while reading the file\n");
        return 0;
    }

    int found = 0;
    while (fread(&currplayer, sizeof(struct player), 1, fp))
    {
        if (strcmp(playerName, currplayer.playerName) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}

int checkplayerID(char *playerName)
{
    FILE *fp;
    int i = 0;
    fp = fopen("playerData.txt", "r");
    struct player currplayer;
    while (fread(&currplayer, sizeof(struct player), 1, fp))
    {
        if (strcasecmp(playerName, currplayer.playerName) == 0)
        {
            i++;
        }
    }
    fclose(fp);
    return i;
}

int getPreviousCheckPoint(char *userName)
{
    FILE *fp;
    fp = fopen("playerData.txt", "r");
    struct player currplayer;
    while (fread(&currplayer, sizeof(struct player), 1, fp))
    {
        if (strcmp(userName, currplayer.playerName) == 0)
        {
            fclose(fp);
            return currplayer.level;
        }
    }
    fclose(fp);
    return -1;
}
