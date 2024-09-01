#include <stdio.h>
#include <stdlib.h>

#include "types.h"

const char *colors[] = {"Red", "Green", "Yellow", "Blue"};

Square *red_start = NULL, *green_start = NULL, *yellow_start = NULL, *blue_start = NULL;
Square *Bhawana, *Kotuwa, *Pitakotuwa;
int mysteryCell = -1;

// Create 4 players
Player redPlayer;
Player greenPlayer;
Player yellowPlayer;
Player bluePlayer;

//creating board
Square *create_ludo_board()
{
    Square *head = NULL, *tail = NULL, *prev_square = NULL;

    for (int i = 1; i <= 52; i++)
    {
        Square *new_square = (Square *)malloc(sizeof(Square));
        new_square->square_number = i;
        new_square->next = NULL;
        new_square->pieceCount = 0;

        //Initialize speacial cells on board
        if (new_square->square_number == 1)
        {
            red_start = new_square;
        }
        else if (new_square->square_number == 14)
        {
            green_start = new_square;
        }
        else if (new_square->square_number == 27)
        {
            yellow_start = new_square;
        }
        else if (new_square->square_number == 40)
        {
            blue_start = new_square;
        }
        else if (new_square->square_number == 36)
        {
            Bhawana = new_square;
        }
        else if (new_square->square_number == 2)
        {
            Kotuwa = new_square;
        }
        else if (new_square->square_number == 21)
        {
            Pitakotuwa = new_square;
        }

        if (head == NULL)
        {
            head = tail = new_square;
        }
        else
        {
            new_square->prev = tail;
            tail->next = new_square;
            tail = new_square;
        }
    }

    // Connect the last square to the first and first to last to form a circular list
    tail->next = head;
    head->prev = tail;
    return head;
}

// Initialize pieces
void initializePlayers()
{
    // Initialize Red Player
    for (int i = 0; i < TOTAL_PIECES; i++)
    {
        redPlayer.pieces[i].isInPlay = BASE;
        redPlayer.pieces[i].direction = 0; // Assuming 0 is clockwise
        redPlayer.pieces[i].current_square = red_start;
        redPlayer.pieces[i].isCapture = 0;
        redPlayer.pieces[i].approachCellPassed = 0;
        redPlayer.pieces[i].homeStraight = 0;
        redPlayer.pieces[i].isSick = -1;
        redPlayer.pieces[i].isSickNumber = 0;
        redPlayer.pieces[i].brief = -1;
    }
    redPlayer.approachCell = 51;
    redPlayer.colour = 'r'; // Example value
    redPlayer.hasWon = 4;

    // Initialize Green Player
    for (int i = 0; i < TOTAL_PIECES; i++)
    {
        greenPlayer.pieces[i].isInPlay = BASE;
        greenPlayer.pieces[i].direction = 0; // Assuming 0 is clockwise
        greenPlayer.pieces[i].current_square = green_start;
        greenPlayer.pieces[i].isCapture = 1;
        greenPlayer.pieces[i].approachCellPassed = 0;
        greenPlayer.pieces[i].homeStraight = 0;
        greenPlayer.pieces[i].isSick = -1;
        greenPlayer.pieces[i].isSickNumber = 0;
        greenPlayer.pieces[i].brief = -1;
    }
    greenPlayer.approachCell = 12;
    greenPlayer.colour = 'g'; // Example value
    greenPlayer.hasWon = 4;

    // Initialize Yellow Player
    for (int i = 0; i < TOTAL_PIECES; i++)
    {
        yellowPlayer.pieces[i].isInPlay = BASE;
        yellowPlayer.pieces[i].direction = 0; // Assuming 0 is clockwise
        yellowPlayer.pieces[i].current_square = yellow_start;
        yellowPlayer.pieces[i].isCapture = 0;
        yellowPlayer.pieces[i].approachCellPassed = 0;
        yellowPlayer.pieces[i].homeStraight = 0;
        yellowPlayer.pieces[i].isSick = -1;
        yellowPlayer.pieces[i].isSickNumber = 0;
        yellowPlayer.pieces[i].brief = -1;
    }
    yellowPlayer.approachCell = 25; // Example value
    yellowPlayer.colour = 'y';
    yellowPlayer.hasWon = 4;
    // Initialize Blue Player
    for (int i = 0; i < TOTAL_PIECES; i++)
    {
        bluePlayer.pieces[i].isInPlay = BASE;
        bluePlayer.pieces[i].direction = 0; // Assuming 0 is clockwise
        bluePlayer.pieces[i].current_square = blue_start;
        bluePlayer.pieces[i].isCapture = 0;
        bluePlayer.pieces[i].approachCellPassed = 0;
        bluePlayer.pieces[i].homeStraight = 0;
        bluePlayer.pieces[i].isSick = -1;
        bluePlayer.pieces[i].isSickNumber = 0;
        bluePlayer.pieces[i].brief = -1;
    }
    bluePlayer.approachCell = 38; // Example value
    bluePlayer.colour = 'b';
    bluePlayer.hasWon = 4;
}

/*---------------------------------------------------------------------------------------
                                      Dice and Coin
    ---------------------------------------------------------------------------------------------*/

//Roll the dice
int diceValue()
{
    return rand() % 6 + 1;
}

// Coin toss
int direction()
{
    int val = rand() % 2 + 1;
    return val - 1;
}


// Determines the first player based on the highest dice roll
int firstPlayer()
{
    // dice rolled by all 4 players and store their respective dice values
    int firstRoundValues[4];
    for (int i = 0; i < 4; i++)
    {
        firstRoundValues[i] = diceValue();
    }

    // Printing the dice rolls for each player
    printf("Red rolls %d\n", firstRoundValues[0]);
    printf("Green rolls %d\n", firstRoundValues[1]);
    printf("Yellow rolls %d\n", firstRoundValues[2]);
    printf("Blue rolls %d\n", firstRoundValues[3]);

    printf("\n");

    int max = 0;
    int maxIndex;

    for (int i = 0; i < 4; i++)
    {
        if (firstRoundValues[i] > max)
        {
            max = firstRoundValues[i];
            maxIndex = i;
        }
    }

    printf("%s player has the highest roll and will begin the game \n", colors[maxIndex]);
    // print the first player based on the highest dice roll

    printf("\n");

    return maxIndex;
}

int isWon(Player player)  // check pieces are in home
{
    for (int i = 0; i < 4; i++)
    {
        if (player.pieces[i].isInPlay == 3)
        {
            return 1;
        }
    }
    return 0;

}


/*---------------------------------------------------------------------------------------
                                      Mystry cell & teleporting
    ---------------------------------------------------------------------------------------------*/

void setMysteryCell(int roundNumber)
{

    if (roundNumber == 2)   // Appear mistry cell after 2 rounds.
    {   // Generate mistry cells randomly.
        mysteryCell = rand() % 52 + 1;
        printf("A mystery cell has spwaned in location %d and will be at this location for the next four rounds \n", mysteryCell);
    }
    else if ((roundNumber - 2) % 4 == 0)    // appear mistry cells after every 4 rounds
    {   //2,6,10,14 round
        int randomCell = rand() % 52 + 1;
        int isTrue = 0;
        while (!isTrue)
        {
            if (mysteryCell != randomCell)
            {
                mysteryCell = randomCell;
                printf("A mystery cell has spwaned in location %d and will be at this location for the next four rounds \n", mysteryCell);
            }
            isTrue = 1;
        }
    }
}

int teleport(Player *player, int pieceNumber, const char *color)
{

    int speacialCell = rand() % 6 + 1;
    /*  1. Bhawana 
        2. Kotuwa 
        3. Pita-Kotuwa 
        4. Base 
        5. X of the piece colour 
        6. Approach of the piece colour
    */

    if (speacialCell == 1)          // To Bhawana
    {
        int random = rand() % 2 + 1;
        /*1.Sick
          2.Energied
          */
        player->pieces[pieceNumber].current_square = Bhawana;

        player->pieces[pieceNumber].isSick = random;
        player->pieces[pieceNumber].isSickNumber = 0;

        if (random == 1)    //Sick
        {
            printf("Color %s piece %d feels sick , and movement speed halves \n", color, pieceNumber);
        }
        else if (random == 2)   //Energized
        {
            printf("Color %s piece %d feels Energized , and movement speed doubles \n", color, pieceNumber);
        }
        printf("%s piece %d teleported to Bhawana \n", color, pieceNumber);
    }
    else if (speacialCell == 2)     // To Kotuwa
    {
        player->pieces[pieceNumber].current_square = Kotuwa;
        printf("%s piece %d teleported to Kotuwa \n", color, pieceNumber);

        player->pieces[pieceNumber].brief = 0;
        printf("Color %s piece %d attends briefing and cannot move for four Rounds. \n", color, pieceNumber);
    }
    else if (speacialCell == 3)     // To Pita-Kotuwa
    {
        player->pieces[pieceNumber].current_square = Pitakotuwa;
        printf("%s piece %d teleported to PitaKotuwa \n", color, pieceNumber);
        if (player->pieces[pieceNumber].direction == 0)
        {
            player->pieces[pieceNumber].direction == 1;
            printf("The color %s piece %d, which was moving clockwise, has changed to moving counterclockwise \n", color, pieceNumber + 1);
        }
        else
        {
            player->pieces[pieceNumber].current_square = Kotuwa;
            printf("The color %s piece %d is moving in a counterclockwise direction.Teleporting from Pita-Kotuwa", color, pieceNumber + 1);
        }
    }
    else if (speacialCell == 4)     // To Base
    { // returns to the start square.
        if (player->colour == 'r')      // To red base
        {
            player->pieces[pieceNumber].current_square = red_start;
            player->pieces[pieceNumber].isInPlay = 1;
            printf("%s piece %d teleported to X \n", color, pieceNumber + 1);
        }
        else if (player->colour == 'g') // To green base
        {

            player->pieces[pieceNumber].current_square = green_start;
            player->pieces[pieceNumber].isInPlay = 1;

            printf("%s piece %d teleported to X  \n", color, pieceNumber + 1);
        }
        else if (player->colour == 'y') // To yellow base
        {
            player->pieces[pieceNumber].current_square = yellow_start;
            player->pieces[pieceNumber].isInPlay = 1;
            printf("%s piece %d teleported to X \n", color, pieceNumber + 1);
        }

        else                            // To blue base
        {
            player->pieces[pieceNumber].current_square = blue_start;
            player->pieces[pieceNumber].isInPlay = 1;
            printf("%s piece %d teleported to X \n", color, pieceNumber + 1);
        }
    }
    else if (speacialCell == 5)     // To X
    { // return to the base square
        if (player->colour == 'r')
        {
            player->pieces[pieceNumber].current_square = red_start;
            player->pieces[pieceNumber].isInPlay = -1;
            printf("%s piece %d teleported to Base \n", color, pieceNumber + 1);
        }
        else if (player->colour == 'g')
        {
            player->pieces[pieceNumber].current_square = green_start;
            player->pieces[pieceNumber].isInPlay = -1;
            printf("%s piece %d teleported to Base \n", color, pieceNumber + 1);
        }
        else if (player->colour == 'y')
        {
            player->pieces[pieceNumber].current_square = yellow_start;
            player->pieces[pieceNumber].isInPlay = -1;
            printf("%s piece %d teleported to Base  \n", color, pieceNumber + 1);
        }
        else
        {
            player->pieces[pieceNumber].current_square = blue_start;
            player->pieces[pieceNumber].isInPlay = -1;
            printf("%s piece %d teleported to Base \n", color, pieceNumber + 1);
        }
    }
    else if (speacialCell == 6)     // To Appoach
    {

        player->pieces[pieceNumber].current_square->square_number = player->approachCell;
        printf("%s piece %d teleported to approach cell \n", color, pieceNumber + 1);
    }
}


/*---------------------------------------------------------------------------------------
                                      PLAYER CHARACTERISTICS
    ---------------------------------------------------------------------------------------------*/

// eventhough red piece moved to the homestraight still we have to update isSick and isSickNumber maybe
void redMoves(int diceValue)
{

    printf("Color Red rolled %d . \n", diceValue);

    //  first look for a red piece can capture a piece
    int captured = 0;
    for (int i = 0; i < 4; i++)

    {

        if (redPlayer.pieces[i].isInPlay == 1)

        {

            if (redPlayer.pieces[i].isSick == 1 && redPlayer.pieces[i].isSickNumber < 4)
            {
                diceValue = diceValue / 2;
            }
            if (redPlayer.pieces[i].isSick == 2 && redPlayer.pieces[i].isSickNumber < 4)
            {
                diceValue = diceValue * 2;
            }

            Square *temp = redPlayer.pieces[i].current_square;
            int went_homestraight = 0;
            for (int j = 0; j < diceValue; j++)
            {

                if (redPlayer.pieces[i].isCapture && temp->square_number == redPlayer.approachCell)
                {
                    went_homestraight = 1;
                    break;
                }
                if (redPlayer.pieces[i].direction == 0)
                {
                    temp = temp->next;
                }
                else
                {
                    temp = temp->prev;
                }
            }

            if (went_homestraight)
            {
                continue;
            }

            for (int j = 0; j < 4; j++)
            {
                if (greenPlayer.pieces[j].isInPlay == 1 && greenPlayer.pieces[j].current_square == temp)
                {
                    greenPlayer.pieces[j].isInPlay = BASE;
                    greenPlayer.pieces[j].current_square = green_start;
                    captured = 1;
                    printf("R%d lands on square %d , captures color G%d , and returns it to the base.", i + 1, temp->square_number, j + 1);
                    if (redPlayer.pieces[i].isSick > 0)
                    {
                        redPlayer.pieces[i].isSickNumber++;
                        if (redPlayer.pieces[i].isSickNumber == 4)
                        {

                            redPlayer.pieces[i].isSick = -1;
                            redPlayer.pieces[i].isSickNumber = 0;
                        }
                    }

                    break;
                }

                if (bluePlayer.pieces[j].isInPlay == 1 && bluePlayer.pieces[j].current_square == temp)
                {
                    bluePlayer.pieces[j].isInPlay = BASE;
                    bluePlayer.pieces[j].current_square = blue_start;
                    captured = 1;
                    printf("R%d lands on square %d , captures color B%d , and returns it to the base.", i + 1, temp->square_number, j + 1);
                    if (redPlayer.pieces[i].isSick > 0)
                    {
                        redPlayer.pieces[i].isSickNumber++;
                        if (redPlayer.pieces[i].isSickNumber == 4)
                        {

                            redPlayer.pieces[i].isSick = -1;
                            redPlayer.pieces[i].isSickNumber = 0;
                        }
                    }
                    break;
                }

                if (yellowPlayer.pieces[j].isInPlay == 1 && yellowPlayer.pieces[j].current_square == temp)
                {
                    yellowPlayer.pieces[j].isInPlay = BASE;
                    yellowPlayer.pieces[j].current_square = yellow_start;
                    captured = 1;
                    printf("R%d lands on square %d , captures color Y%d , and returns it to the base.", i + 1, temp->square_number, j + 1);
                    if (redPlayer.pieces[i].isSick > 0)
                    {
                        redPlayer.pieces[i].isSickNumber++;
                        if (redPlayer.pieces[i].isSickNumber == 4)
                        {

                            redPlayer.pieces[i].isSick = -1;
                            redPlayer.pieces[i].isSickNumber = 0;
                        }
                    }
                    break;
                }
            }

            if (captured)
            {
                redPlayer.pieces[i].isCapture = 1;
                redPlayer.pieces[i].current_square = temp;
                break;
            }
        }
    }

    if (captured == 0)
    {

        int empty_base = 1;
        for (int i = 0; i < 4; i++)
        {

            if (diceValue == 6 && redPlayer.pieces[i].isInPlay == -1)
            {
                redPlayer.pieces[i].isInPlay = 1;
                int directionValue = direction(); // Determining the direction of the piece
                redPlayer.pieces[i].direction = directionValue;
                printf("Color Red moves piece R%d to the starting point. \n", i + 1);
                int standard = 0;
                int base_count = 0;
                for (int j = 0; j < 4; j++)
                {
                    if (redPlayer.pieces[j].isInPlay == 1)
                    {
                        standard++;
                    }
                    else if (redPlayer.pieces[j].isInPlay == -1)
                    {
                        base_count++;
                    }
                }
                printf("Color Red now has %d/4 pieces on the board and %d/4 pieces on the base.\n", standard, base_count);
                empty_base = 0;
                break;
            }
        }

        if (empty_base)
        {

            for (int i = 0; i < 4; i++)
            {

                // check for dice value and wanted value for go home
                if (redPlayer.pieces[i].isInPlay == 2 && diceValue == (6 - redPlayer.pieces[i].homeStraight))
                {
                    redPlayer.pieces[i].isInPlay = 3;
                    printf("Color Red Player moves piece R%d to home successfully! \n", i + 1);
                    break;
                }

                // check for blocks
                if (redPlayer.pieces[i].isInPlay == 1)
                {
                    Square *temp = redPlayer.pieces[i].current_square;
                    int went_homestraight = 0;
                    for (int j = 0; j < diceValue; j++)
                    {

                        if (redPlayer.pieces[i].isCapture && temp->square_number == redPlayer.approachCell)
                        {
                            went_homestraight = 1;
                            redPlayer.pieces[i].homeStraight = diceValue - j;
                            if (redPlayer.pieces[i].homeStraight == 6)
                            {
                                redPlayer.pieces[i].isInPlay = 3;
                                printf("Color Red Player moves piece R%d to home successfully! \n", i + 1);
                            }
                            else
                            {
                                redPlayer.pieces[i].isInPlay = 2;
                                printf("Color Red Player moves piece R%d to %d homestraight by %d. \n", i + 1, redPlayer.pieces[i].homeStraight, diceValue);
                            }
                            break;
                        }

                        if (redPlayer.pieces[i].direction == 0)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            temp = temp->prev;
                        }
                    }

                    if (!went_homestraight)
                    {
                        int isteleport = 0;
                        int old_square = redPlayer.pieces[i].current_square->square_number;
                        if (temp->square_number == mysteryCell)
                        {
                            teleport(&redPlayer, i, "red");
                            isteleport = 1;
                        }

                        if (!isteleport)
                        {
                            redPlayer.pieces[i].current_square = temp;

                            if (redPlayer.pieces[i].direction == 0)
                            {
                                printf("Color Red Player moves piece R%d from location %d to %d by %d units in clockwise direction.\n", i + 1, old_square, redPlayer.pieces[i].current_square->square_number, diceValue);
                            }
                            else
                            {
                                printf("Color Red Player moves piece R%d from location %d to %d by %d units in counter-clockwise direction.\n", i + 1, old_square, redPlayer.pieces[i].current_square->square_number, diceValue);
                            }

                            break;
                        }
                    }
                }
            }
        }
    }
}

void greenMoves(int diceValue)
{

    printf("Color Green rolled %d . \n", diceValue);
    // first checks a piece can create a block
    int createBlock = 0;
    for (int i = 0; i < 4; i++)
    {

        if (greenPlayer.pieces[i].isInPlay == 1)
        {

            if (greenPlayer.pieces[i].isSick == 1 && greenPlayer.pieces[i].isSickNumber < 4)
            {
                diceValue = diceValue / 2;
            }

            if (greenPlayer.pieces[i].isSick == 2 && greenPlayer.pieces[i].isSickNumber < 4)
            {
                diceValue = diceValue * 2;
            }

            Square *temp = greenPlayer.pieces[i].current_square;
            int went_homestraight = 0;
            for (int j = 0; j < diceValue; j++)
            {

                if (greenPlayer.pieces[i].isCapture && temp->square_number == greenPlayer.approachCell)
                {
                    went_homestraight = 1;
                    break;
                }

                if (greenPlayer.pieces[i].direction == 0)
                {
                    temp = temp->next;
                }
                else
                {
                    temp = temp->prev;
                }
            }

            if (went_homestraight)
            {
                continue;
            }

            // checking other pieces are in the landing Square
            for (int j = 0; j < 4; j++)
            {
                if (j == i)
                {
                    continue;
                }

                if (greenPlayer.pieces[j].isInPlay == 1 && greenPlayer.pieces[j].current_square == temp)
                {
                    createBlock = 1;
                    break;
                }
            }

            if (createBlock)
            {
                // pieces i and j created a block
                greenPlayer.pieces[i].current_square = temp;
                if (greenPlayer.pieces[i].isSick > 0)
                {
                    greenPlayer.pieces[i].isSickNumber++;
                    if (greenPlayer.pieces[i].isSickNumber == 4)
                    {

                        greenPlayer.pieces[i].isSick = -1;
                        greenPlayer.pieces[i].isSickNumber = 0;
                    }
                }
                break;
            }
        }
    }

    if (!createBlock)
    {

        int empty_base = 1;
        for (int i = 0; i < 4; i++)
        {

            if (diceValue == 6 && greenPlayer.pieces[i].isInPlay == BASE)
            {
                greenPlayer.pieces[i].isInPlay = 1;
                int directionValue = direction(); // Determining the direction of the piece
                greenPlayer.pieces[i].direction = directionValue;

                printf("Color Green moves piece G%d to the starting point. \n", i + 1);
                int standard = 0;
                int base_count = 0;
                for (int j = 0; j < 4; j++)
                {
                    if (greenPlayer.pieces[j].isInPlay == 1)
                    {
                        standard++;
                    }
                    else if (greenPlayer.pieces[j].isInPlay == -1)
                    {
                        base_count++;
                    }
                }
                printf("Color Green now has %d/4 pieces on the board and %d/4 pieces on the base.\n", standard, base_count);
                empty_base = 0;
                break;
            }
        }

        if (empty_base)
        {

            // Green will move first piece
            for (int i = 0; i < 4; i++)
            {

                // check for dice value and wanted value for go home
                if (greenPlayer.pieces[i].isInPlay == 2 && diceValue == (6 - greenPlayer.pieces[i].homeStraight))
                {
                    greenPlayer.pieces[i].isInPlay = 3;
                    printf("Color Green Player moves piece G%d to home successfully! \n", i + 1);
                    break;
                }

                // check for blocks
                if (greenPlayer.pieces[i].isInPlay == 1)
                {
                    Square *temp = greenPlayer.pieces[i].current_square;
                    int went_homestraight = 0;
                    for (int j = 0; j < diceValue; j++)
                    {

                        if (greenPlayer.pieces[i].isCapture && temp->square_number == greenPlayer.approachCell)
                        {
                            went_homestraight = 1;
                            greenPlayer.pieces[i].homeStraight = diceValue - j;
                            if (greenPlayer.pieces[i].homeStraight == 6)
                            {
                                greenPlayer.pieces[i].isInPlay = 3;
                                printf("Color Green Player moves piece G%d to home successfully! \n", i + 1);
                            }
                            else
                            {
                                greenPlayer.pieces[i].isInPlay = 2;
                                printf("Color Green Player moves piece G%d to %d homestraight by %d. \n", i + 1, greenPlayer.pieces[i].homeStraight, diceValue);
                            }
                            break;
                        }

                        if (greenPlayer.pieces[i].direction == 0)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            temp = temp->prev;
                        }
                    }

                    if (!went_homestraight)
                    {
                        int old_square = greenPlayer.pieces[i].current_square->square_number;
                        greenPlayer.pieces[i].current_square = temp;
                        printf("Color Green Player moves piece G%d from location %d to %d by %d units in %s direction.\n",
                               i + 1,
                               old_square,
                               greenPlayer.pieces[i].current_square->square_number,
                               diceValue,
                               greenPlayer.pieces[i].direction == 0 ? "clockwise" : "anti-clockwise");

                        break;
                    }
                }
            }
        }
    }
}

void yellowMoves(int diceValue)
{

    printf("Color Yellow rolled %d . \n", diceValue);
    // First checks if yellow can take any piece out.
    int empty_base = 1;

    for (int i = 0; i < 4; i++)
    {

        if (diceValue == 6 && yellowPlayer.pieces[i].isInPlay == -1)
        {
            yellowPlayer.pieces[i].isInPlay = 1;
            int directionValue = direction(); // Determining the direction of the piece
            yellowPlayer.pieces[i].direction = directionValue;
            printf("Color Yellow moves piece Y%d to the starting point. \n", i + 1);
            int standard = 0;
            int base_count = 0;
            for (int j = 0; j < 4; j++)
            {
                if (yellowPlayer.pieces[j].isInPlay == 1)
                {
                    standard++;
                }
                else if (yellowPlayer.pieces[j].isInPlay == -1)
                {
                    base_count++;
                }
            }
            printf("Color Yellow now has %d/4 pieces on the board and %d/4 pieces on the base.\n", standard, base_count);
            empty_base = 0;
            break;
        }
    }

    if (empty_base)
    {
        int isTeleport = 0;

        for (int i = 0; i < 4; i++)
        {
            if (bluePlayer.pieces[i].current_square->square_number + diceValue == mysteryCell)
            {
                teleport(&bluePlayer, i, "blue");
                isTeleport = 1;
                break;
            }
        }

        if (!isTeleport)
        {

            int captured = 0;
            for (int i = 0; i < 4; i++)
            {

                if (yellowPlayer.pieces[i].isInPlay == 1)
                {

                    if (yellowPlayer.pieces[i].isSick == 1 && yellowPlayer.pieces[i].isSickNumber < 4)
                    {
                        diceValue = diceValue / 2;
                    }

                    if (yellowPlayer.pieces[i].isSick == 2 && yellowPlayer.pieces[i].isSickNumber < 4)
                    {
                        diceValue = diceValue * 2;
                    }

                    Square *temp = yellowPlayer.pieces[i].current_square;
                    int went_homestraight = 0;
                    for (int j = 0; j < diceValue; j++)
                    {

                        if (yellowPlayer.pieces[i].isCapture && temp->square_number == yellowPlayer.approachCell)
                        {
                            went_homestraight = 1;
                            break;
                        }

                        if (yellowPlayer.pieces[i].direction == 0)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            temp = temp->prev;
                        }
                    }

                    // If it passes approach cell after capturing, then it can't capture again
                    if (went_homestraight)
                    {
                        continue;
                    }

                    for (int j = 0; j < 4; j++)
                    {
                        if (greenPlayer.pieces[j].isInPlay == 1 && greenPlayer.pieces[j].current_square == temp)
                        {
                            greenPlayer.pieces[j].isInPlay = BASE;
                            greenPlayer.pieces[j].current_square = green_start;
                            captured = 1;
                            printf("Y%d lands on square %d , captures color G%d , and returns it to the base.\n", i + 1, temp->square_number, j + 1);

                            if (yellowPlayer.pieces[i].isSick > 0)
                            {
                                yellowPlayer.pieces[i].isSickNumber++;
                                if (yellowPlayer.pieces[i].isSickNumber == 4)
                                {

                                    yellowPlayer.pieces[i].isSick = -1;
                                    yellowPlayer.pieces[i].isSickNumber = 0;
                                }
                            }

                            break;
                        }

                        if (bluePlayer.pieces[j].isInPlay == 1 && bluePlayer.pieces[j].current_square == temp)
                        {
                            bluePlayer.pieces[j].isInPlay = BASE;
                            bluePlayer.pieces[j].current_square = blue_start;
                            captured = 1;
                            printf("Y%d lands on square %d , captures color B%d , and returns it to the base.\n", i + 1, temp->square_number, j + 1);

                            if (yellowPlayer.pieces[i].isSick > 0)
                            {
                                yellowPlayer.pieces[i].isSickNumber++;
                                if (yellowPlayer.pieces[i].isSickNumber == 4)
                                {

                                    yellowPlayer.pieces[i].isSick = -1;
                                    yellowPlayer.pieces[i].isSickNumber = 0;
                                }
                            }

                            break;
                        }

                        if (redPlayer.pieces[j].isInPlay == 1 && redPlayer.pieces[j].current_square == temp)
                        {
                            redPlayer.pieces[j].isInPlay = BASE;
                            redPlayer.pieces[j].current_square = red_start;
                            captured = 1;
                            printf("Y%d lands on square %d , captures color R%d , and returns it to the base.\n", i + 1, temp->square_number, j + 1);

                            if (yellowPlayer.pieces[i].isSick > 0)
                            {
                                yellowPlayer.pieces[i].isSickNumber++;
                                if (yellowPlayer.pieces[i].isSickNumber == 4)
                                {

                                    yellowPlayer.pieces[i].isSick = -1;
                                    yellowPlayer.pieces[i].isSickNumber = 0;
                                }
                            }

                            break;
                        }
                    }

                    if (captured)
                    {
                        yellowPlayer.pieces[i].isCapture = 1;
                        yellowPlayer.pieces[i].current_square = temp;
                        break;
                    }
                }
            }

            // if couldn't capture a piece
            if (captured == 0)
            {

                // check if any in homestraight can be moved
                int homestraight_moved = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (yellowPlayer.pieces[i].isInPlay == 2 && diceValue == (6 - yellowPlayer.pieces[i].homeStraight))
                    {
                        yellowPlayer.pieces[i].isInPlay = 3;
                        printf("Color Yellow Player moves piece Y%d to home successfully! \n", i + 1);
                        homestraight_moved = 1;
                        break;
                    }
                }

                if (homestraight_moved == 0)
                {
                    // check the closes piece to home
                    int closest_distance = 9999;
                    int closest_piece = -1;
                    for (int i = 0; i < 4; i++)
                    {
                        if (yellowPlayer.pieces[i].isInPlay == 1)
                        {
                            int homeDistance = 0;
                            if (yellowPlayer.pieces[i].current_square->square_number <= yellowPlayer.approachCell)
                            {

                                homeDistance = yellowPlayer.approachCell - yellowPlayer.pieces[i].current_square->square_number;
                                if (yellowPlayer.pieces[i].direction == 1)
                                {
                                    homeDistance = 52 - homeDistance;
                                }
                            }
                            else
                            {
                                homeDistance = 52 - (yellowPlayer.pieces[i].current_square->square_number - yellowPlayer.approachCell);
                                if (yellowPlayer.pieces[i].direction == 1)
                                {
                                    homeDistance = 52 - homeDistance;
                                }
                            }
                            if (homeDistance < closest_distance)
                            {
                                closest_piece = i;
                                closest_distance = homeDistance;
                            }
                        }
                    }

                    // If there's a closes piece detected
                    if (closest_piece != -1)
                    {
                        Square *temp = yellowPlayer.pieces[closest_piece].current_square;
                        int went_homestraight = 0;
                        for (int j = 0; j < diceValue; j++)
                        {
                            // check if piece can goto home straight
                            if (yellowPlayer.pieces[closest_piece].isCapture && temp->square_number == yellowPlayer.approachCell)
                            {
                                went_homestraight = 1;
                                yellowPlayer.pieces[closest_piece].homeStraight = diceValue - j;
                                if (yellowPlayer.pieces[closest_piece].homeStraight == 6)
                                {
                                    yellowPlayer.pieces[closest_piece].isInPlay = 3;
                                    printf("Color Yellow Player moves piece Y%d to home successfully! \n", closest_piece + 1);
                                }
                                else
                                {
                                    yellowPlayer.pieces[closest_piece].isInPlay = 2;
                                    printf("Color Yellow Player moves piece Y%d to %d homestraight by %d. \n", closest_piece + 1, yellowPlayer.pieces[closest_piece].homeStraight, diceValue);
                                }
                                break;
                            }

                            if (yellowPlayer.pieces[closest_piece].direction == 0)
                            {
                                temp = temp->next;
                            }
                            else
                            {
                                temp = temp->prev;
                            }
                        }
                        // Piece moves normally
                        if (!went_homestraight)
                        {
                            int old_square = yellowPlayer.pieces[closest_piece].current_square->square_number;
                            yellowPlayer.pieces[closest_piece].current_square = temp;
                            printf("Color Yellow Player moves piece Y%d from location %d to %d by %d units in %s direction.\n",
                                   closest_piece + 1,
                                   old_square,
                                   yellowPlayer.pieces[closest_piece].current_square->square_number,
                                   diceValue,
                                   yellowPlayer.pieces[closest_piece].direction == 0 ? "clockwise" : "anti-clockwise");
                        }
                    }
                }
            }
        }
    }
}

void blueMoves(int diceValue)
{

    printf("Color Blue rolled %d . \n", diceValue);
    // First checks if yellow can take any piece out.
    int isTeleport = 0;

    for (int i = 0; i < 4; i++)
    {
        if (bluePlayer.pieces[i].current_square->square_number + diceValue == mysteryCell)
        {
            teleport(&bluePlayer, i, "blue");
            isTeleport = 1;
            break;
        }
    }

    if (!isTeleport)
    {

        int empty_base = 1;
        for (int i = 0; i < 4; i++)
        {

            if (diceValue == 6 && bluePlayer.pieces[i].isInPlay == -1)
            {
                bluePlayer.pieces[i].isInPlay = 1;
                int directionValue = direction();
                bluePlayer.pieces[i].direction = directionValue;
                printf("Color Blue moves piece Y%d to the starting point. \n", i + 1);
                int standard = 0;
                int base_count = 0;
                for (int j = 0; j < 4; j++)
                {
                    if (bluePlayer.pieces[j].isInPlay == 1)
                    {
                        standard++;
                    }
                    else if (bluePlayer.pieces[j].isInPlay == -1)
                    {
                        base_count++;
                    }
                }
                printf("Color Yellow now has %d/4 pieces on the board and %d/4 pieces on the base.\n", standard, base_count);
                empty_base = 0;
                break;
            }
        }

        if (empty_base)
        {

            // Checks if yellow can capture any piece
            int captured = 0;
            for (int i = 0; i < 4; i++)
            {

                if (bluePlayer.pieces[i].isInPlay == 1)
                {

                    Square *temp = bluePlayer.pieces[i].current_square;
                    int went_homestraight = 0;
                    for (int j = 0; j < diceValue; j++)
                    {

                        if (bluePlayer.pieces[i].isCapture && temp->square_number == bluePlayer.approachCell)
                        {
                            went_homestraight = 1;
                            break;
                        }

                        if (bluePlayer.pieces[i].direction == 0)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            temp = temp->prev;
                        }
                    }

                    // If it passes approach cell after capturing, then it can't capture again
                    if (went_homestraight)
                    {
                        continue;
                    }

                    for (int j = 0; j < 4; j++)
                    {
                        if (greenPlayer.pieces[j].isInPlay == 1 && greenPlayer.pieces[j].current_square == temp)
                        {
                            greenPlayer.pieces[j].isInPlay = BASE;
                            greenPlayer.pieces[j].current_square = green_start;
                            captured = 1;
                            printf("Y%d lands on square %d , captures color G%d , and returns it to the base.\n", i + 1, temp->square_number, j + 1);
                            break;
                        }

                        if (yellowPlayer.pieces[j].isInPlay == 1 && yellowPlayer.pieces[j].current_square == temp)
                        {
                            yellowPlayer.pieces[j].isInPlay = BASE;
                            yellowPlayer.pieces[j].current_square = yellow_start;
                            captured = 1;
                            printf("Y%d lands on square %d , captures color B%d , and returns it to the base.\n", i + 1, temp->square_number, j + 1);
                            break;
                        }

                        if (redPlayer.pieces[j].isInPlay == 1 && redPlayer.pieces[j].current_square == temp)
                        {
                            redPlayer.pieces[j].isInPlay = BASE;
                            redPlayer.pieces[j].current_square = red_start;
                            captured = 1;
                            printf("Y%d lands on square %d , captures color R%d , and returns it to the base.\n", i + 1, temp->square_number, j + 1);
                            break;
                        }
                    }

                    if (captured)
                    {
                        bluePlayer.pieces[i].isCapture = 1;
                        bluePlayer.pieces[i].current_square = temp;
                        break;
                    }
                }
            }

            // if couldn't capture a piece
            if (captured == 0)
            {

                // check if any in homestraight can be moved
                int homestraight_moved = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (bluePlayer.pieces[i].isInPlay == 2 && diceValue == (6 - bluePlayer.pieces[i].homeStraight))
                    {
                        bluePlayer.pieces[i].isInPlay = 3;
                        printf("Color blue Player moves piece Y%d to home successfully! \n", i + 1);
                        homestraight_moved = 1;
                        break;
                    }
                }

                if (homestraight_moved == 0)
                {
                    // check the closes piece to home
                    int closest_distance = 9999;
                    int closest_piece = -1;
                    for (int i = 0; i < 4; i++)
                    {
                        if (bluePlayer.pieces[i].isInPlay == 1)
                        {
                            int homeDistance = 0;
                            if (bluePlayer.pieces[i].current_square->square_number <= bluePlayer.approachCell)
                            {
                                homeDistance = bluePlayer.approachCell - bluePlayer.pieces[i].current_square->square_number;
                                if (bluePlayer.pieces[i].direction == 1)
                                { // Distance if anti clockwise
                                    homeDistance = 52 - homeDistance;
                                }
                            }
                            else
                            {
                                homeDistance = 52 - (bluePlayer.pieces[i].current_square->square_number - bluePlayer.approachCell);
                                if (bluePlayer.pieces[i].direction == 1)
                                { // Distance if anti clockwise
                                    homeDistance = 52 - homeDistance;
                                }
                            }
                            if (homeDistance < closest_distance)
                            {
                                closest_piece = i;
                                closest_distance = homeDistance;
                            }
                        }
                    }

                    // If there's a closes piece detected
                    if (closest_piece != -1)
                    {
                        Square *temp = bluePlayer.pieces[closest_piece].current_square;
                        int went_homestraight = 1;
                        for (int j = 0; j < diceValue; j++)
                        {
                            // check if piece can goto home straight
                            if (bluePlayer.pieces[closest_piece].isCapture && temp->square_number == bluePlayer.approachCell)
                            {
                                went_homestraight = 0;
                                bluePlayer.pieces[closest_piece].homeStraight = diceValue - j;
                                if (bluePlayer.pieces[closest_piece].homeStraight == 6)
                                {
                                    bluePlayer.pieces[closest_piece].isInPlay = 3;
                                    printf("Color blue Player moves piece Y%d to home successfully! \n", closest_piece + 1);
                                }
                                else
                                {
                                    bluePlayer.pieces[closest_piece].isInPlay = 2;
                                    printf("Color blue Player moves piece Y%d to %d homestraight by %d. \n", closest_piece + 1, bluePlayer.pieces[closest_piece].homeStraight, diceValue);
                                }
                                break;
                            }

                            if (bluePlayer.pieces[closest_piece].direction == 0)
                            {
                                temp = temp->next;
                            }
                            else
                            {
                                temp = temp->prev;
                            }
                        }
                        // Piece moves normally
                        if (went_homestraight)
                        {
                            int old_square = bluePlayer.pieces[closest_piece].current_square->square_number;
                            bluePlayer.pieces[closest_piece].current_square = temp;
                            if (bluePlayer.pieces[closest_piece].direction == 0)
                            {
                                printf("Color blue Player moves piece Y%d from location %d to %d by %d units in clockwise direction.\n", closest_piece + 1, old_square, bluePlayer.pieces[closest_piece].current_square->square_number, diceValue);
                            }
                            else
                            {
                                printf("Color blue Player moves piece Y%d from location %d to %d by %d units in counter-clockwise direction.\n", closest_piece + 1, old_square, bluePlayer.pieces[closest_piece].current_square->square_number, diceValue);
                            }
                        }
                    }
                }
            }
        }
    }
}


// Capture pieces
int isCapture(Player player, int diceValue)
{

    int captured = 0;
    for (int i = 0; i < 4; i++)
    {

        if (player.pieces[i].isInPlay == 1)
        {

            Square *temp = player.pieces[i].current_square;
            int went_homestraight = 0;
            for (int j = 0; j < diceValue; j++)
            {

                if (player.pieces[i].isCapture && temp->square_number == player.approachCell)
                {
                    went_homestraight = 1;
                    break;
                }

                temp = temp->next;
            }

            if (went_homestraight)
            {
                continue;
            }

            for (int j = 0; j < 4; j++)
            {
                if (greenPlayer.pieces[j].isInPlay == 1 && greenPlayer.pieces[j].current_square == temp && player.colour != 'g')
                {
                    greenPlayer.pieces[j].isInPlay = BASE;
                    greenPlayer.pieces[j].current_square = green_start;
                    captured = 1;
                    printf("%c%d lands on square %d , captures color G%d , and returns it to the base.", player.colour, i + 1, temp->square_number, j + 1);
                    break;
                }

                if (bluePlayer.pieces[j].isInPlay == 1 && bluePlayer.pieces[j].current_square == temp && greenPlayer.colour != 'g')
                {
                    bluePlayer.pieces[j].isInPlay = BASE;
                    bluePlayer.pieces[j].current_square = blue_start;
                    captured = 1;
                    printf("%c%d lands on square %d , captures color B%d , and returns it to the base.", player.colour, i + 1, temp->square_number, j + 1);
                    break;
                }

                if (yellowPlayer.pieces[j].isInPlay == 1 && yellowPlayer.pieces[j].current_square == temp && greenPlayer.colour != 'g')
                {
                    yellowPlayer.pieces[j].isInPlay = BASE;
                    yellowPlayer.pieces[j].current_square = yellow_start;
                    captured = 1;
                    printf("%c%d lands on square %d , captures color Y%d , and returns it to the base.", player.colour, i + 1, temp->square_number, j + 1);
                    break;
                }

                if (redPlayer.pieces[j].isInPlay == 1 && redPlayer.pieces[j].current_square == temp && greenPlayer.colour != 'g')
                {
                    redPlayer.pieces[j].isInPlay = BASE;
                    redPlayer.pieces[j].current_square = red_start;
                    captured = 1;
                    printf("%c%d lands on square %d , captures color R%d , and returns it to the base.", player.colour, i + 1, temp->square_number, j + 1);
                    break;
                }
            }

            if (captured)
            {
                player.pieces[i].isCapture = 1;
                player.pieces[i].current_square = temp;
                break;
            }
        }
    }

    return captured;
}

// Handele player turn & consecutive six dice turns 
void diceTurns(int current_player)
{
    int a = current_player % 4;
    int dice;
    switch (a){
    case 0:
        dice = diceValue();  // roll dice
        redMoves(dice);
        if (dice == 6)
        {

            dice = diceValue(); // roll dice
            redMoves(dice);

            if (dice == 6)
            {

                dice = diceValue();  // roll dice

                if (dice != 6)
                {
                    redMoves(dice);
                }
            }
        }
        break;
    case 1:
        dice = diceValue();  // roll dice
        greenMoves(dice);
        if (dice == 6)
        {

            dice = diceValue();  // roll dice
            greenMoves(dice);

            if (dice == 6)
            {

                dice = diceValue();  // roll dice

                if (dice != 6)
                {
                    greenMoves(dice);
                }
            }
        }
        break;
    case 2:
        dice = diceValue();  // roll dice
        yellowMoves(dice);
        if (dice == 6)
        {

            dice = diceValue(); // roll dice
            yellowMoves(dice);

            if (dice == 6)
            {

                dice = diceValue();  // roll dice

                if (dice != 6)
                {
                    yellowMoves(dice);
                }
            }
        }
        break;

    case 3:
        dice = diceValue();
        blueMoves(dice);
        if (dice == 6)
        {

            dice = diceValue();
            blueMoves(dice);

            if (dice == 6)
            {

                dice = diceValue();

                if (dice != 6)
                {
                    blueMoves(dice);
                }
            }
        }
        break;
    }
}


/*---------------------------------------------------------------------------------------
                                      One round of game loop
    ---------------------------------------------------------------------------------------------*/

//loop for one round
void gameRound(int firstIndex)
{

    int currentPlayer = firstIndex % 4;
    int turns = 0;
    int dice;

    while (turns < 4)   // one round completed
    {

        diceTurns(currentPlayer % 4);
        turns++;
        currentPlayer++;
    }
}


/*  check pieces on board
    check pieces on base

    check current speacial positions on board (base,standerd cell,home straight,home)
    check weather all pieces are in home (win condition)
*/
int haswon = 1;
int roundSummary(int i)
{
    printf("\nRound %d Details : \n", i);

    /*+=++++++++++++++++++++++++++++ RED PLAYER +++++++++++++++++++++++++++++++*/

    // how many pieces on base & board
    int inBase = 0;
    int total_finished = 0; // Declared outside the loop
    for (int i = 0; i < 4; i++)
    {
        if (redPlayer.pieces[i].isInPlay == -1)
        {
            inBase++;
        }
    }
    printf("Color Red player now has %d/4 pieces on the board and %d/4 pieces on the base.\n", 4 - inBase, inBase);
    printf("===================================================================================\n");
    printf("Location of pieces Red:\n");
    printf("===================================================================================\n");

    for (int i = 0; i < 4; i++)
    {
        if (redPlayer.pieces[i].isInPlay == -1)         // Piece is in base
        {
            printf("Piece %d: Base\n", i + 1);
        }
        else if (redPlayer.pieces[i].isInPlay == 1)     // Piece is in starnderd cells
        {
            printf("Piece %d: %d\n", i + 1, redPlayer.pieces[i].current_square->square_number);
        }
        else if (redPlayer.pieces[i].isInPlay == 2)     // Piece is in home straight
        {
            printf("Piece %d: %d (Home Straight)\n", i + 1, redPlayer.pieces[i].homeStraight);
        }
        else if (redPlayer.pieces[i].isInPlay == 3)     // Piece is in home
        {
            printf("Piece %d: In Home\n", i + 1);
            total_finished++;
        }
    }
    // Win condition of player
    if (total_finished == 4)
    {
        printf("Red Has Finished The game!!!\n");
        if (redPlayer.hasWon == 4)      //true if all pieces are on home
        {
            redPlayer.hasWon = haswon;
            haswon++;
        }
    }
    printf("\n");



    /*+=++++++++++++++++++++++++++++ GREEN PLAYER +++++++++++++++++++++++++++++++*/

    // how many pieces on base & board
    inBase = 0;
    total_finished = 0; // Reset for the next player
    for (int i = 0; i < 4; i++)
    {
        if (greenPlayer.pieces[i].isInPlay == -1)
        {
            inBase++;
        }
    }
    printf("Color Green player now has %d/4 pieces on the board and %d/4 pieces on the base.\n", 4 - inBase, inBase);
    printf("===================================================================================\n");
    printf("Location of pieces Green:\n");
    printf("===================================================================================\n");

    for (int i = 0; i < 4; i++)
    {
        if (greenPlayer.pieces[i].isInPlay == -1)           // Piece is in base
        {
            printf("Piece %d: Base\n", i + 1);
        }
        else if (greenPlayer.pieces[i].isInPlay == 1)       // Piece is in starnderd cells
        {
            printf("Piece %d: %d\n", i + 1, greenPlayer.pieces[i].current_square->square_number);
        }
        else if (greenPlayer.pieces[i].isInPlay == 2)       // Piece is in home straight
        {
            printf("Piece %d: %d (Home Straight)\n", i + 1, greenPlayer.pieces[i].homeStraight);
        }
        else if (greenPlayer.pieces[i].isInPlay == 3)       // Piece is in home
        {
            printf("Piece %d: In Home\n", i + 1);
            total_finished++;
        }
    }

    // Win condition of player
    if (total_finished == 4)        //true if all pieces are on home
    {
        printf("Green Has Finished The game!!!\n");
        if (greenPlayer.hasWon == 4)
        {
            greenPlayer.hasWon = haswon;
            haswon++;
        }
    }
    printf("\n");



    /*++++++++++++++++++++++++++++++ YELLOW PLAYER +++++++++++++++++++++++++++++++*/

    // how many pieces on base & board
    inBase = 0;
    total_finished = 0; // Reset for the next player
    for (int i = 0; i < 4; i++)
    {
        if (yellowPlayer.pieces[i].isInPlay == -1)
        {
            inBase++;
        }
    }
    printf("Color Yellow player now has %d/4 pieces on the board and %d/4 pieces on the base.\n", 4 - inBase, inBase);
    printf("===================================================================================\n");
    printf("Location of pieces Yellow:\n");
    printf("===================================================================================\n");

    for (int i = 0; i < 4; i++)
    {
        if (yellowPlayer.pieces[i].isInPlay == -1)          // Piece is in base
        {
            printf("Piece %d: Base\n", i + 1);
        }
        else if (yellowPlayer.pieces[i].isInPlay == 1)      // Piece is in starnderd cells
        {
            printf("Piece %d: %d\n", i + 1, yellowPlayer.pieces[i].current_square->square_number);
        }
        else if (yellowPlayer.pieces[i].isInPlay == 2)      // Piece is in home straight
        {
            printf("Piece %d: %d (Home Straight)\n", i + 1, yellowPlayer.pieces[i].homeStraight);
        }
        else if (yellowPlayer.pieces[i].isInPlay == 3)      // Piece is in home
        {
            printf("Piece %d: In Home\n", i + 1);
            total_finished++;
        }
    }

    // Win condition of player
    if (total_finished == 4)        //true if all pieces are on home
    {
        printf("Yellow Has Finished The game!!!\n");
        if (yellowPlayer.hasWon == 4)
        {
            yellowPlayer.hasWon = haswon;
            haswon++;
        }
    }
    printf("\n");



    /*+=++++++++++++++++++++++++++++ BLUE PLAYER +++++++++++++++++++++++++++++++*/

    // how many pieces on base & board
    inBase = 0;
    total_finished = 0;
    for (int i = 0; i < 4; i++)
    {
        if (bluePlayer.pieces[i].isInPlay == -1)
        {
            inBase++;
        }
    }
    printf("Color Blue player now has %d/4 pieces on the board and %d/4 pieces on the base.\n", 4 - inBase, inBase);
    printf("===================================================================================\n");
    printf("Location of pieces Blue:\n");
    printf("===================================================================================\n");

    for (int i = 0; i < 4; i++)
    {
        if (bluePlayer.pieces[i].isInPlay == -1)            // Piece is in base
        {
            printf("Piece %d: Base\n", i + 1);
        }
        else if (bluePlayer.pieces[i].isInPlay == 1)        // Piece is in starnderd cells
        {
            printf("Piece %d: %d\n", i + 1, bluePlayer.pieces[i].current_square->square_number);
        }
        else if (bluePlayer.pieces[i].isInPlay == 2)        // Piece is in home straight
        {
            printf("Piece %d: %d (Home Straight)\n", i + 1, bluePlayer.pieces[i].homeStraight);
        }
        else if (bluePlayer.pieces[i].isInPlay == 3)        // Piece is in home
        {
            printf("Piece %d: In Home\n", i + 1);
            total_finished++;
        }
    }

    // Win condition of player
    if (total_finished == 4)        //true if all pieces are on home
    {
        printf("Blue Has Finished The game!!!\n");
        if (bluePlayer.hasWon == 4)
        {
            bluePlayer.hasWon = haswon;
            haswon++;
        }
    }
    printf("\n");

    return 0;
}


/*---------------------------------------------------------------------------------------
                                      Main Game Function
    ---------------------------------------------------------------------------------------------*/

void runThis()
{
    create_ludo_board();
    initializePlayers();
    intro();
    int currentPlayer = firstPlayer();

    int i = 1;

    // Game Loop until all players have finished the game
    while (haswon < 4)
    {
        printf("\n!!!! Round %d Starts !!!!\n", i);
        setMysteryCell(i - 1);                          // Generating mystery cells
        gameRound(currentPlayer);                       // Runs one round
        printf("\n");
        roundSummary(i);                                // Print summery of one round
        i++;                                            // Incriment number of rounds
        printf("\n");
    }
    result();                                           // Print final result
}


/*---------------------------------------------------------------------------------------
                                      Print massages (start & end)
    ---------------------------------------------------------------------------------------------*/

//print piece names
void intro()
{
    printf("The red player has 04 pieces named R1,R2,R3,R4\n");
    printf("The green player has 04 pieces named G1,G2,G3,G4\n");
    printf("The yellow player has 04 pieces named Y1,Y2,Y3,Y4\n");
    printf("The blue player has 04 pieces named B1,B2,B3,B4\n\n");
}

//Places of players (1st,2nd,3rd,4th)
void result()
{
    printf("\n===========================================================================\n");
    printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Results of LUDO CS !!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("\n Red Player : Place %d", redPlayer.hasWon);
    printf("\n Blue Player : Place %d", bluePlayer.hasWon);
    printf("\n Yellow Player : Place %d", yellowPlayer.hasWon);
    printf("\n Green Player : Place %d\n", greenPlayer.hasWon);
}
