#include <stdio.h>
#include <stdlib.h>
#include <string.h> // LIBRARY FOR STRINGS
#include <stdbool.h> // LIBRARY FOR 'BOOL'
#include <unistd.h> // LIBRARY FOR 'SLEEP' FUNCTION

struct Playlist { //DEFINING PLAYLIST STRUCUTRE
    char *songName;
    struct Playlist *link;
};

bool isEmpty(struct Playlist *head) { //VERIFY IF PLAYLIST IS EMPTY
    if (head == NULL)
        return true;
    else
        return false;
}

bool checkDuplicate(struct Playlist *head, char songName[]) { //VERIFY IF A SONG IS ALREADY IN PLAYLIST
    struct Playlist *ptr = head;
    while(ptr != NULL) {
        if(strcmp(ptr->songName, songName) == 0) {
            printf("This song is already in your playlist.\n");
            return true;
        }
        ptr = ptr->link;
    }
    return false;
}

struct Playlist *createNode(char songName[]) { //CREATE NODE
    struct Playlist *newNode = (struct Playlist*)malloc(sizeof(struct Playlist));

    newNode->songName = strdup(songName);
    newNode->link = NULL;

    return newNode;
}

bool addSong(struct Playlist **head, char songName[]) { //ADD SONG TO PLAYLIST
    if(checkDuplicate(*head, songName))
        return true;

    struct Playlist *newSong = createNode(songName);
    if (*head == NULL)
        *head = newSong;

    else {
        struct Playlist *temp = *head;
        while (temp->link != NULL)
            temp = temp->link;
        temp->link = newSong;
    }
    printf("Song successfully added.\n");
}

void showPlaylist(struct Playlist *head) { //SHOWING PLAYLIST
    if (isEmpty(head))
        printf("Your playlist is empty.\n");

    else {
        struct Playlist *ptr = head;
        int i = 1;
        printf("Your current playlist is: \n");
        while (ptr != NULL) {
            printf("%d. %s", i, ptr->songName);
            printf("\n");
            ptr = ptr->link;
            i += 1;
        }
    }
}

int countSongs(struct Playlist *head) { //COUNT HOW MANY SONGS ARE IN THE PLAYLIST
    int count = 0;
    if (head == NULL)
        count = 0;

    struct Playlist *ptr = head;
    while (ptr != NULL) {
        count += 1;
        ptr = ptr->link;
    }
    return count;
}

struct Playlist *deleteSong(struct Playlist **head) { //DELETE A SONG FROM THE PLAYLIST
    if (isEmpty(*head)) {
        printf("Your playlist is empty.\n");
        return NULL;
    }

    showPlaylist(*head);
    printf("Which song would you like to delete?\n");
    int userChoice;
    scanf("%d", &userChoice);

    if (userChoice == 1) {
        struct Playlist *temp = *head;
        *head = (*head) -> link;
        free(temp);
        temp = NULL;
        printf("Song successfully deleted.\n");
        showPlaylist(*head);

        return *head;
    }
    else if (userChoice == countSongs(*head)) {
        struct Playlist *temp = *head;
        while(temp->link->link != NULL)
            temp = temp->link;
        free(temp->link);
        temp->link = NULL;
        printf("Song successfully deleted.\n");
        showPlaylist(*head);

        return *head;
    }
    else {
        struct Playlist *temp1 = *head;
        while (userChoice-1 != 1) {
            temp1 = temp1->link;
            userChoice -= 1;
        }
        struct Playlist *temp2 = temp1->link;
        temp1->link = temp1->link->link;
        free(temp2);
        temp2 = NULL;
        printf("Song successfully deleted.\n");
        showPlaylist(*head);

        return *head;
    }

}

void editSong(struct Playlist *head) { //MODIFY AN ALREADY EXISTING SONG
    if (isEmpty(head)) {
        printf("No songs to be edited.\n");
        return;
    }

    int count = countSongs(head);
    int songNumber;
    showPlaylist(head);
    printf("What song would you like to edit?\n");
    scanf("%d", &songNumber);
    if (songNumber < 1 || songNumber > count) {
        printf("Invalid number.\n");
        return;
    }

    struct Playlist *temp = head;
    while(songNumber != 1) {
        temp = temp->link;
        songNumber -= 1;
    }

    char newSongName[256];
    printf("Enter the new song name: \n");
    getchar(); //WE CONSUME THE POSSIBLE '\n' THAT COULD BE IN THE BUFFER
    scanf("%[^\n]s", newSongName); //
    free(temp->songName);
    temp->songName = malloc(strlen(newSongName) + 1); //WE ALLOCATE MORE MEMORY IN CASE THE NEW SONG HAS A LONGER NAME THAN THE PREVIOUS
    strcpy(temp->songName, newSongName);
    printf("Song name successfully edited.\n");

}

void searchSong(struct Playlist *head) { //SEARCH A SONG IN THE PLAYLIST
    if (isEmpty(head)) {
        printf("Your playlist is empty.\n");
        return;
    }
    
    char songSearchName[256];
    printf("What song would you like to find?\n");
    fgets(songSearchName, sizeof(songSearchName), stdin); 
    songSearchName[strcspn(songSearchName, "\n")] = '\0'; 

    struct Playlist *ptr = head;
    int songIndex = 1;
    while (ptr != NULL) {
        if(strcmp(ptr->songName, songSearchName) == 0) {
            printf("Song %s successfully found at index %d.\n", songSearchName, songIndex);
            return;
        }
    ptr = ptr->link;
    songIndex += 1;
    }
    printf("Song %s is not in your playlist.\n", songSearchName);
}

struct Playlist *deletePlaylist(struct Playlist **head) { //DELETE ENTIRE PLAYLIST
    if(isEmpty(*head)) {
        printf("Your playlist is empty.\n");
        return NULL;
    }

    printf("Deleting playlist...\n");
    struct Playlist *temp = *head;
    while(*head != NULL) {
        *head = (*head)->link;
        free(temp);
        temp = *head;
    }
    sleep(2);
    printf("Playlist successfully deleted.\n");
    return *head;
}

struct Playlist *reversePlaylist(struct Playlist **head) { //INVERTING THE ORDER OF THE SONGS IN THE PLAYLIST
    if(isEmpty(*head)) {
        printf("Your playlist is empty.\n");
        return NULL;
    }

    struct Playlist *previousNode = NULL;
    struct Playlist *nextNode = NULL;

    printf("Reversing playlist...\n");
    while(*head != NULL) {
        nextNode = (*head)->link;
        (*head)->link = previousNode;
        previousNode = *head;
        *head = nextNode;
    }
    *head = previousNode;
    sleep(2);
    printf("Playlist successfully reversed.\n");
    showPlaylist(*head);
    return *head;
}

struct Playlist *moveSong(struct Playlist **head) { //CHANGING THE PLACE OF TWO SONGS
    if(isEmpty(*head)) {
        printf("Your playlist is empty.\n");
        return NULL;
    }

    int count = countSongs(*head);
    showPlaylist(*head);

    int songToBeMoved;
    printf("Which song would you like to move?\n");
    scanf("%d", &songToBeMoved);
    if (songToBeMoved > count || songToBeMoved <= 0) {
        printf("Invalid number.\n");
        return NULL;
    }

    int whereToMove;
    printf("Where would you like to move it to?\n");
    scanf("%d", &whereToMove);
    if (whereToMove > count || whereToMove <= 0) {
        printf("Invalid number.\n");
        return NULL;
    }

    if(songToBeMoved == whereToMove) {
        printf("The song is already at that position.\n");
        return NULL;
    }

    struct Playlist *temp1 = *head;
    while (songToBeMoved != 1) {
        temp1 = temp1->link;
        songToBeMoved -= 1;
    }

    struct Playlist *temp2 = *head;
    while (whereToMove != 1) {
        temp2 = temp2->link;
        whereToMove -= 1;
    }

    printf("Moving song...\n");
    char aux[256];
    strcpy(aux, temp1->songName); 
    strcpy(temp1->songName, temp2->songName); 
    strcpy(temp2->songName, aux); 
    sleep(2);
    printf("Successfully moved song.\n");
    showPlaylist(*head);

    return *head;
}

int main() {
    struct Playlist *head = NULL; 
    int a=0;
    while(1) { //MENU THROUGH WHICH WE SELECT THE DESIRED ACTION.
            printf("Welcome to your playlist!\n Choose one of the options below: \n");
            printf("1. Add song.\n"); // ADD SONG
            printf("2. Delete song.\n"); // DELETE SONG
            printf("3. Edit song.\n"); // MODIFY SONG
            printf("4. Search song.\n"); // SEARCH SONG
            printf("5. Reverse playlist.\n"); // INVERTING SONG ORDER
            printf("6. Move song.\n"); // CHANGING THE PLACE OF TWO SONGS
            printf("7. Show playlist.\n"); // SHOW PLAYLIST
            printf("8. Delete playlist.\n"); // DELETE PLAYLIST
            printf("9. Exit.\n"); // EXIT THE PROGRAM


        int userChoice;
        scanf("%d", &userChoice);
        getchar();

        switch(userChoice) {
            case 1: {
                char songName[256];
                printf("What song would you like to add to your playlist?\n");
                scanf("%[^\n]s", songName);
                addSong(&head, songName);
                break;
            }

            case 2:
                deleteSong(&head);
                break;

            case 3:
                editSong(head);
                break;

            case 4:
                searchSong(head);
                break;

            case 5:
                reversePlaylist(&head);
                break;

            case 6:
                moveSong(&head);
                break;

            case 7:
                showPlaylist(head);
                break;

            case 8:
                deletePlaylist(&head);
                break;

            case 9:
                printf("Have a nice day :)");
                exit(1);

            default:
                exit(1);

        }
    }
}
