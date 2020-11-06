/**
 * the board of the game is a two dimensional array of ints
 * the board of the game has
 * -CHIPS which are numbers above zero and maximum of 9
 * -OBSTACLES which are represented by '#' and have vavlue of -1
 * -LINES which are represented by 'o' and have value of -2
 * -NOTHING which are represented by '.' and have value of 0
 * 
 * the board is less than 27 (rows or columns), because 26 latin characters
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

//auth einai mia domh me onoma Board
typedef struct 
{
//apoteleitai apo tis parakatw metavlites
    int columns;//sthles
    int rows;//grammes
    int difficulty;//duskolia
    int level;//epipedo 
    int chips;//chip
    int score;//skor
    int connected[10];//apo8ikeuontai oi ari8moi twn chip poy sunde8hkan
    int **board;//didiastatos dinamikos pinakas
	//apo8hkeyei to chip kai thn eisodo tou xrhsth gia thn enwsh
    char wires[9][128];
    char message[128];//mhnumata poy provaloyme sto xrhsth
}Board;

//ayth h synarthsh ka8arizei thn cache
void flushStdin() {
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}

//synarthsh random gia tyxaious ari8mous
int my_random(int max) {
    double d;
    int r;
	//ari8moi kinhths upodiastolhs me times apo 0...1
    d = (double)rand() / ((double)RAND_MAX+1);
    d = d*max;
	//afou pallaplasiasoume me to diko mas max to kanoume akeraio ari8mo
    r = (int)d;
    return r;
}

//sunarthsh popBoard
//gemisma pinaka kai arxikopoihsh
int popBoard(Board *board) {
    int i, k, row, column;
	//empodia duskolia epi 5 giati einai 5,10 h 15 kai h duskolia 1,2 h 3
    int obstaclePrcentage = board->difficulty * 5;
	//ta zeugaria apo chip isoute me to level dld gia level 2 exoyme 2 zeugaria chip
    int chipPairs = board->level;
	//upologismos empodiwn oses sthles * grammes * to pososto poy upologisame parapanw
	//pes oti balame 5 sthles 5 grammes kai duskolia 2
	//apotelesma 5*5*10/100
    int numberOfObstacles = board->columns*board->rows*obstaclePrcentage/100;
    
	//edw elegxoume na mhn ksefeugei to zeugos chip panw ap to epipedo pou epileksame
	//analoga me th duskolia ta zeugh einai 3,6 h 9
    if (chipPairs > board->difficulty * 3) {
        chipPairs = board->difficulty * 3;
    }
//diatrexoume ton pinaka ana grammes kai sthles kai gemizoume me 0
    for (i=0; i<board->rows; i++) {
        for (k=0; k<board->columns; k++) {
            board->board[i][k] = 0;
        }
    }
    //osos einai o ari8mos ton empodiwn poy upologisame 
	//tyxaia row kai column kalwntas thn my_random
	//kai topo8etoume ton ari8mo -1 poy einai empodia kai sumvolizontai me #
    for (i=0; i<numberOfObstacles; i++) {
        do {
            row = my_random(board->rows);
            column = my_random(board->columns);
        }while(board->board[row][column]!=0);
        board->board[row][column] = -1;
    }
    //gia osa chip 8eloume kai gia 2 fores thn ka8e fora afou 8eloyme na emfanizontai ana zeugaria
    for (i=1; i<=chipPairs; i++) {
        for (k=0; k<2; k++) {
            do {
			//tuxaies 8eseis me thn my_random
                row = my_random(board->rows);
                column = my_random(board->columns);
            }while(board->board[row][column]!=0);
			//topo8etoume to katallhlo noumero chip
            board->board[row][column] = i;
        }
    }
    //arxikopoioume ton pinaka connected na einai ola ta stoixeia 0
    for (i=0; i<10; i++) {
        board->connected[i] = 0;
    }
    //arxikopoioyme kai edw ton pinaka wires
    for (i=0; i<9; i++) {
        strcpy(board->wires[i],"");
    }
	//8etoume ta chips ths domhs Board = me ton ari8mo chip
    board->chips = chipPairs;
	//strcpy antigrafei to keimeno meta o koma sto board->message ara edw keno mhnyma
	//arxikoppoioume dld kai edw
    strcpy(board->message, "");
    return 0;
}

//sunarthsh create board
//desmeush dunamika xwrou gia ton pinaka
Board createBoard(int columns, int rows, int difficulty){
    int i;
    Board result;
    int **board;
    
    result.level = 0;
//h sunarthsh malloc desmevei xwro sthn mnhmh
    board = (int**)malloc(rows*sizeof(int *));
    if (board == NULL) return result;
    
    for (i=0; i<rows; i++) {
        board[i] = (int *)malloc(columns*sizeof(int));
        if (board[i] == NULL) {
            i--;
            while(i>=0) {
                free(board[i]);
                i--;
            }
            free(board);
            return result;
        }
    }
    //afoy desmeusame ton xwro
	//arxikopoihsh gia na arxisei to paixnidi
    result.level = 1;
    result.columns = columns;
    result.rows = rows;
    result.board = board;
    result.difficulty = difficulty;
    result.score = 0;
    strcpy(result.message, "");
	//edw kaloume thn popBoard gia na gemisei o pinakas me 0 poy einai . kai -1 poy einai #
    if (result.board != NULL) popBoard(&result);
    return result;
}

//synarthsh printBoard
//ektypwsh pinaka opws 8eloume na fainetai sto xrhsth
int printBoard(Board board) {
    int i, k;
    int columns = board.columns;
    int rows = board.rows;
    
    printf("level: %d\n", board.level);
    printf("    ");
    for (k=0; k<columns; k++) {
        printf("%i ", k+1);
    }
    printf("\n");
    printf("____");
    for (k=0; k<columns; k++) {
        printf("__");
    }
    printf("\n");
    for (i=0; i<rows; i++) {
        printf("%c | ", i+65);
        for (k=0; k<columns; k++) {
		//opoy -2 bazoume o pou einai kalwdio
            switch(board.board[i][k]) {
                case -2:
                    printf("o ");
                    break;
					//opou -1 # pou einai empodio
                case -1:
                    printf("# ");
                    break;
					//opou 0 . pou einai keno
                case 0:
                    printf(". ");
                    break;
					//ari8mos chip
                default:
                    printf("%i ", board.board[i][k]);
            }
        }
        printf("\n");
    }
	//ektupwsh poia zeygh chip exoun sunde8ei
    printf("connected chips: ");
    for (i=0; i<9; i++) {
        if (board.connected[i]) printf("%d ", i);
    }
	//ektupwsh skor
    printf("\nYour score is: %d\n", board.score);
    if (strcmp(board.message,"")) printf("%s\n", board.message);
    return 0;
}

//sunarthsh metatophs xarakthra se ari8mo
int char2num(char c) {
    if (c<49) return 0;
    if (c<65) return c-49;
    if (c<97) return c-65;
    return c-97;
}

//synarthsh isInBoard
//elegxei an h eisodos pou phre ap ton xrhsth einai mesa sto tamplo tou paixnidiou
//kai den bgainei ektos orion
int isInBoard(char *point, Board board) {
//estw oti dwsame a3
//point[0] einai einai to a. afairoume a gia na paroume ari8mo
//point[1] einai to 3. afairoume -1 gati arxizei ap to 0
    int row = point[0] - 'a';
    int column = point[1] - '1';
	//edw ginetai o elegxos twn oriwn tou pinaka
    if (row >= 0 && row < board.rows && 
        column >= 0 && column < board.columns) return 1;
    return 0;
}

//elegxei an einai keno dhladh an periexei .
int isempty(char *point, Board board) {
    int row = point[0] - 'a';
    int column = point[1] -'1';
    if (board.board[row][column] == 0) return 1;
    return 0;
}

//allazei thn timh pou uparxei ston pinaka se mia allh
//an einai keno dhladh . h alliws 0 kai 8eloume na ginei kalwdio mpainei h timh -2
void setPoint(Board *board, char *point, int value) {
    int row = point[0] - 'a';
    int column = point[1] -'1';
    board->board[row][column] = value;
}

//pairnei thn timh tou shmeiou
//mporoue etsi na elegsoume an einai chip kai na ginei h sundesh
int getPoint(Board board, char *point) {
    int row = point[0] - 'a';
    int column = point[1] -'1';
    return board.board[row][column];
}

//elegxoume to kalwdio dld thn eisodo pou edwse o xrhsths ap to plhktrologio
int validateWire(Board *board, char *wire) {
    int i = 0;
    int value;
    char point[2];
	//prepei na pame apo ena shmeio se allo dld eisodos estw a3a4
	//strlen metraei tous xarakthres
	//an einai ligoteroi apo 4 provlima, 8eloume toulaxiston 4 xarakthres ap ton xrhsth
    if (strlen(wire)<4) {
        strcpy(board->message,"the wire string is too short");
        return 0;
    }
	//edw elegxoume na mhn einai monos ari8mos dld na mhn exei balei o xrhsths 5
	//px a3a4a
    if (strlen(wire)%2!=0) {
        strcpy(board->message,"the wire string must be odd characters");
        return 0;
    }
    
	//elegxos an einai mesa sto tamplo h kinhsh
    while(wire[i]) {
	//point[0] to gramma point[1] o ari8mos kai auksanoume to i gia na elegxei ola ta shmeia
        point[0] = wire[i];
        i++;
        point[1] = wire[i];
        if (!isInBoard(point, *board)) {//prepei ola ta shmeia na einai entos toy pinaka
            strcpy(board->message, "the wire goes off limits");
            return 0;
        }
        i++;
    }
	//i mhkos eisodou -1 
	//epeidh o pinakas arxizei na metraei ap to 0
	//elegxos an einai diagwnia pou apagoreuetai
    i = strlen(wire)-1;
    while(i>1) {
        if (wire[i] != wire[i-2] && wire[i-1] != wire[i-3]) {//prepei ta shmeia ana 2 na einai katseta-orizontia
            strcpy(board->message,"the wire must be manhatan type");
            return 0;
        }
		//meiwsh i kata 2 gia na elegxoume 
        i-=2;
    }
    
	//edw kaloume thn getPoint pou epistrefei ti timh exei to shmeio
    point[0] = wire[0];
    point[1] = wire[1];
    value = getPoint(*board, point);
    if (value < 1) {//prepei to arxiko shmeio na einai chip
        strcpy(board->message,"the wire's start is not a chip");
        return 0;
    }
	//edw elegxoume ta 2 teleutaia ap th eisodo gia na doume an einai chip
	//kalwntas pali thn getPoint
    point[0] = wire[strlen(wire)-2];
    point[1] = wire[strlen(wire)-1];
    if (getPoint(*board,point) != value) {//prepei to teliko shmeio na einai chip
        strcpy(board->message,"the wire's end must be of the same chip as the start");
        return 0;
    }
	
	//elegxoume na mhn einai to telos kai h arxh to idio
    if (point[0] == wire[0] && point[1] == wire[1]) {//prepei h arxi kai to telos na einai diaforetika shmeia
        strcpy(board->message,"the wire's start and end must be different");
        return 0;
    }
	
    if (board->connected[value] == 1) {//prepei na mhn exei syndethei
        strcpy(board->message,"the chip is allready connected");
        return 0;
    }
    return value;//epistrefei to noymero toy chip poy tha syndethei
}

int nextInPath(Board board, char *path, char *newPoint) {
    static int index =2;
    static char curPoint[2] = {'\0','\0'};
    static char curPath[128] = "";
    int drow = 0, dcolumn = 0;

    if (newPoint==NULL) {//initialize
        strcpy(curPath, path);
        curPoint[0] = path[0];
        curPoint[1] = path[1];
        index = 2;
        return 0;
    }
    while(curPoint[0] == path[index] && curPoint[1] == path[index+1]) {
        index += 2;
    }
    if (path[index] == '\0') return 0;
    
    if (path[index] < curPoint[0]) drow = -1;
    else if (path[index] > curPoint[0]) drow = 1;
    else if (path[index+1] < curPoint[1]) dcolumn = -1;
    else if (path[index+1] > curPoint[1]) dcolumn = 1;
    curPoint[0] = curPoint[0] + drow;
    curPoint[1] = curPoint[1] + dcolumn;
    newPoint[0] = curPoint[0];
    newPoint[1] = curPoint[1];
    
    if (index == (strlen(path) - 2) && newPoint[0] == path[index] && newPoint[1] == path[index+1]) {
        return 0;
    }
    return 1;
}

int connect(Board *board, char *path) {
    int result;
    char newPoint[2];
    
    if (!nextInPath(*board, path, newPoint)) return 1;
    if (!isempty(newPoint, *board)){
        strcpy(board->message, "your wire has obstacle or chip or other wire or loops");
        return 0;
    }
	//kaloume setPoint kai allazoume ta 0 se -2 pou einai kalwdio dhladh o
    setPoint(board, newPoint, -2);
	//anadromh, kaloume thn idia sunarthsh mesa sto swma ths sunartshs
    result = connect(board, path);
	//an kati paei la8os den allazei kati bazei 0
    if (!result) setPoint(board, newPoint, 0);
    return result;
}

int undo(Board *board) {
    int i=0;
    char nextPoint[2];
    char path[128] = "";
    while(strcmp(board->wires[i], "") != 0) {i++;}
    if (i == 0) {
        strcpy(board->message, "nothing to undo");
        return 0;
    }
    i--;
	//antegrapse to wires sto path
    strcpy(path, board->wires[i]);
	//keno to wire
    strcpy(board->wires[i], "");
    nextInPath(*board, path, NULL);//initialize
	//bale 0 gia na emfanistoyn .
    while(nextInPath(*board, path, nextPoint)) {
        setPoint(board, nextPoint, 0);
    }
    return 1;
}

// h reset apla ksanafortwnei ton pinaka
//me ta empodia kai ta chip se alles 8eseis
void reset(Board *board){
    popBoard(board);
}

int wire(Board *board, char *path) {
    int i = 0;
    int result = 0;
    //elegxos gia to kalwdia oti ola einai kala
    if (!(result = validateWire(board,path))) {
        return 0;
    }
    nextInPath(*board, path, NULL);//initialize
    //elegxos gia thn sundesh
    if (!connect(board, path)) return 0;
    //antigrafh tou path sto wire kai epistrofh apotelesmatos
    while(strcmp(board->wires[i], "") != 0) {i++;}
    strcpy(board->wires[i], path);
    return result;
}
//epomenh pista me perissotera chip
int levelUp(Board *board) {
    int i=0;
    //int score = board.score;
	//an den exoun sun8e8ei ta chip
    for (i=1; i<=board->chips; i++) {
        if (board->connected[i] == 0) return 0;
    }
	//an sunde8hkan
    strcpy(board->message,"level completed");
	//ektupwsh pinaka kai aukshsh level
    printBoard(*board);
    board->level++;
    popBoard(board);
    strcpy(board->message,"");
    return 1;
}
//menu
//zhtaei ap ton xrhsth poses grammes kai sthles
//thn duskolia
//kalei thn createBoard pou desmeuei th mnhmh
Board menu(){
    int rows = 0;
    int columns = 0;
    int difficulty = 0;
    Board result;
    while(1) {
        printf("how many rows (5-26): ");
        scanf("%d", &rows);
        flushStdin();
        if (rows>4 && rows<27) break;
    }
    while(1) {
        printf("how many columns (5-9): ");
        scanf("%d", &columns);
        flushStdin();
        if (columns>4 && columns<10) break;
    }
    while(1) {
        printf("1. Easy\n");
        printf("2. Medium\n");
        printf("3. Hard\n");
        printf("choose dificulty (1-3): ");
        scanf("%d", &difficulty);
        flushStdin();
        if (difficulty>0 && difficulty<4) break;
    }
    result = createBoard(columns, rows, difficulty);
    return result;
}


//kuria sunarthsh
//argc kai argv einai gia na stelnoume parametrous se ena programma
int main(int argc, char **argv)
{
    int i;
	Board board;
    int chip;
    char command[128];
    int commandSize=127;
    
    //scoring parameters
    int s_wire = 100;
    int level_factor = 50; 
    
    int s_undo = -100;
    int undo_factor = 150;//percentage
    int undoCount=0;
    
    int s_reset = -100;
    int reset_factor = 150;
    int resetCount = 0;
    
    //scoring parameters
    
	//arxikopoihsh random gia tuxaioys ari8mous
    srand(time(NULL));
	//kaleitai h menu gia na dwsei o xrhsths grammes sthles duskolia
	//kai ayth me th seira ths na kalesei thn create gia desmeush mnhmhs
    board = menu();
    
	//elegxos an kati phge la8os
    if (board.level == 0) {
        printf("memory problem, bye...\n");
        return 0;
    }
    
    while(1) {
	//kaleitai h print pou tupwnei ayto poy blepei o xrhsths
        printBoard(board);
        strcpy(board.message, "");
        printf("Wire or give command:");
		//dexetai times ap to plhktrologio giayto stdin
		//apo8hkeuei sto command me megisto mege8os commandSize
		//edw to valame 127
        fgets(command,commandSize,stdin);
		//den 8eloume to \0
        command[strlen(command)-1] = '\0';
        while(command[i]) {
		//metatroph se peza
            command[i] = tolower(command[i]);
            i++;
        }
		//strcmp sugkrinei meta to koma an einai idio me prin
		//dinei 0 an einai
		//opote an ddwseis reset epistrefei 0
		//ara kaleitai h reset kai upologizetai to skor kai auksanetai kata 1 h fora pou to kaleses
        if (strcmp(command,"reset")==0){
            reset(&board);
            resetCount++;
			//upologismos skor meta to reset
			//uparxon skor+(-100)*150*poses fores to kaleses/100
            board.score += s_reset*reset_factor*resetCount/100;
            continue;
        }
		//elegxei an egrapses undo
        if (strcmp(command,"undo")==0){
            if (undo(&board)){
                undoCount++;
				//upologismos skor
                board.score += s_undo*undo_factor*undoCount/100;
            }
            continue;
        }
		//elegxos an edwses exit
        if (strcmp(command,"exit")==0){
            printf("you reached level %d and your score is %d!!!!\n", board.level, board.score);
            break;
        }
        if ((chip = wire(&board, command))) {
            board.connected[chip] = 1;
            board.score += (s_wire + board.level*level_factor) * board.difficulty;
            levelUp(&board);
            continue;
        }
    }
    //apeley8erwsh mhnmhs
    for (i=0;i<board.rows; i++) {
        free(board.board[i]);
    }
    free(board.board);
    
	return 0;
}
