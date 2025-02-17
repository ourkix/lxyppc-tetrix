#include "Tetris.h"

#define     MATRIX_SIZE     32
#define     MAT_ROW_CNT     20
#define     MAT_COL_CNT     10
#define     MATRIX_START    4
#define     MATRIX_END      (MATRIX_START+MAT_ROW_CNT)
#if MATRIX_SIZE < (MATRIX_END + 1)
#error  Buffer too small
#endif

#define     MAP_MASK        0xFFFF
#define     MAP_OFFSET      3
#define     MAT_ROW_MASK     ((1<<MAT_COL_CNT) - 1)

#define     PATTEN_CNT       7
#define     COLOR_CNT        8

typedef enum
{
    TA_Left,
    TA_Right,
    TA_Rotate,
    TA_Down,
    TA_None,
}TetrisAction;

#define     _ExtendBit(x)        ( ((x&1)<<24) | ((x&2)<<15) | ((x&4)<<6) | ((x&8)>>3))
const   unsigned long   BitExtend[16] =
{
    _ExtendBit(0x0),_ExtendBit(0x1),_ExtendBit(0x2),_ExtendBit(0x3),
    _ExtendBit(0x4),_ExtendBit(0x5),_ExtendBit(0x6),_ExtendBit(0x7),
    _ExtendBit(0x8),_ExtendBit(0x9),_ExtendBit(0xA),_ExtendBit(0xB),
    _ExtendBit(0xC),_ExtendBit(0xD),_ExtendBit(0xE),_ExtendBit(0xF),
};

typedef    unsigned char    PattenMap[4];

typedef union   MatrixLine
{
    unsigned long   raw32[4];
    unsigned short  raw16[8];
    unsigned char   raw8[16];
    struct  {
        unsigned char  Data[12];        //12
        unsigned short BitMap;          //2
        unsigned char  preLine;         //1
        unsigned char  nextLine;        //1  = total 16
    };
}MatrixLine;

typedef union  BlockDesc
{
    unsigned long   raw32[8];           //32
    unsigned short  raw16[16];
    unsigned char   raw8[32];
    struct{
        unsigned char   BlockData[16];  //16
        unsigned short  BlockBits[4];   //8
        const PattenMap*  patten;       //4
        signed   char  x;               //1
        signed   char  y;               //1
        unsigned char  rotate;          //1
        signed   char  color;           //1
    };
}BlockDesc;

const PattenMap pattens[PATTEN_CNT*4] = 
{
    // Box
    {0x00,0x06,0x06,0x00},{0x00,0x06,0x06,0x00},
    {0x00,0x06,0x06,0x00},{0x00,0x06,0x06,0x00},
    // L
    {0x04,0x04,0x06,0x00},{0x00,0x07,0x04,0x00},
    {0x06,0x02,0x02,0x00},{0x00,0x02,0x0E,0x00},
    // J
    {0x02,0x02,0x06,0x00},{0x00,0x04,0x07,0x00},
    {0x06,0x04,0x04,0x00},{0x00,0x0E,0x02,0x00},
    // Z
    {0x00,0x06,0x03,0x00},{0x02,0x06,0x04,0x00},
    {0x00,0x06,0x03,0x00},{0x02,0x06,0x04,0x00},
    // S
    {0x00,0x03,0x06,0x00},{0x04,0x06,0x02,0x00},
    {0x00,0x03,0x06,0x00},{0x04,0x06,0x02,0x00},
    // T
    {0x00,0x07,0x02,0x00},{0x02,0x06,0x02,0x00},
    {0x00,0x02,0x07,0x00},{0x02,0x03,0x02,0x00},
    // line
    {0x04,0x04,0x04,0x04},{0x00,0x0F,0x00,0x00},
    {0x04,0x04,0x04,0x04},{0x00,0x0F,0x00,0x00},
};

MatrixLine      matrix[MATRIX_SIZE];

BlockDesc       curBlock;
BlockDesc       nextBlock;
unsigned    int  score = 0;
unsigned    int  level = 0;
void            ScoreUp(int line);

void    InitialMatrix();
void    CreateBlock(BlockDesc* block);
int     CheckBlock(BlockDesc* block, TetrisAction action);
int     MoveBlock(BlockDesc* block, TetrisAction action);
#define    CopyBlock(des, src)  ((des)->raw32 = (scr)->raw32)

int     DropBlock(BlockDesc* block);
unsigned char   firstLine = 0;
unsigned char   curLine = 0;
void    GetCurrentLine(signed char pos);

int     TetrisPlay(int param)
{
    static int flag = 0;
    if(!flag){
        flag = 1;
        InitialMatrix();
        CreateBlock(&curBlock);

        // Create next block
        CreateBlock(&nextBlock);
        GetCurrentLine(curBlock.y);
        DisplayScoreLevel();
    }

    //while(1)
    {
        int key;
        TetrisAction action;
        DebugDump();

        // Check valid
        if(!CheckBlock(&curBlock,TA_None)){
            // Game over
            printf("Game over!\n");
        }

        key = GetKey();
        switch(key){
            case KEY_LEFT:
                action = TA_Left;
                score++;
                break;
            case KEY_RIGHT:
                action = TA_Right;
                score+=10;
                break;
            case KEY_UP:
                action = TA_Rotate;
                score+=100;
                break;
            case KEY_DOWN:
                action = TA_Down;
                score+=1000;
                break;
            case KEY_PAUSE:
                break;
            default:
                action = TA_Down;
                break;
        }
        if(CheckBlock(&curBlock,action)){
            MoveBlock(&curBlock,action);
        }else if(action == TA_Down){
            ScoreUp(DropBlock(&curBlock));
            CopyBlock(&curBlock,&nextBlock);
            CreateBlock(&nextBlock);
            GetCurrentLine(curBlock.y);
        }
    }
    return 0;
}

void  GetCurrentLine(signed char pos)
{
    curLine = firstLine;
    while(pos<0){
        pos++;
        curLine = matrix[curLine].preLine;
    }
}

void    InitialMatrix()
{
    unsigned int i;
    for(i=0;i<MATRIX_SIZE;i++){
        matrix[i].raw32[0] = 0;
        matrix[i].raw32[1] = 0;
        matrix[i].raw32[2] = 0;
        matrix[i].raw32[3] = 0;
        matrix[i].nextLine = i + 1;
        matrix[i].preLine = i - 1;
        //             ___11bits__ __10bits__ ___11bits__
        //             0123456789A 1234567890 0123456789A  // Ten bits
        //             0123456789A BCDEF01234 56789ABCDEF
        // Bit patten  11111111111 0000000000 11111111111
        if(i>=MATRIX_START && i<MATRIX_END){
            matrix[i].BitMap = MAP_MASK - (MAT_ROW_MASK<<MAP_OFFSET);
        }else{
            matrix[i].BitMap = MAP_MASK;
        }
    }
    firstLine = MATRIX_START;
}

void    CreateBlock(BlockDesc* block)
{
    int rnd = Rand16();
    int color = Rand16()&0x0F;
    int i;
    rnd &= 7;
    if(rnd>=PATTEN_CNT){
        rnd = Rand16()&3;
    }
    block->patten = pattens + rnd*4;
    block->rotate = Rand16()&3;
    block->x = 3;
    block->y = 0;
    block->color = 1;color;
    rnd = 1;
    for(i=0;i<4;i++){
        unsigned char pat = block->patten[block->rotate][i];
        block->raw32[i] = BitExtend[pat]*(color+1);
        if(pat && rnd){
            block->y -= i;
            rnd = 0;
        }
    }
    if(rnd){
        printf("Error!");
    }
}

void    CopyBlock(BlockDesc* des, const BlockDesc* src)
{
    int i;
    for(i=0;i<sizeof(BlockDesc)/4;i++){
        des->raw32[i] = src->raw32[i];
    }
}

int     CheckBlock(BlockDesc* block, TetrisAction action)
{
    int bx = block->x;
    unsigned int br = block->rotate;
    int i;
    unsigned char line = curLine;
    switch(action){
        case TA_Left:
            bx--;
            break;
        case TA_Right:
            bx++;
            break;
        case TA_Rotate:
            br = (br + 1) & 3;
            break;
        case TA_Down:
            line = matrix[line].nextLine;
            break;
        default:
            break;
    }
    
    for(i=0;i<4;i++){
        unsigned long blockMap = block->patten[br][i];
        blockMap<<=(MAP_OFFSET+MAT_COL_CNT-bx-4);
        if(matrix[line].BitMap & blockMap){
            return 0;
        }
        line = matrix[line].nextLine;
    }
    return 1;
}

int     MoveBlock(BlockDesc* block, TetrisAction action)
{
    int i;
    switch(action){
        case TA_Left:
            block->x--;
            break;
        case TA_Right:
            block->x++;
            break;
        case TA_Rotate:
            block->rotate=(block->rotate+1)&3;
            break;
        case TA_Down:
            block->y++;
            curLine = matrix[curLine].nextLine;
            break;
    }
    for(i=0;i<4;i++){
        unsigned char pat = block->patten[block->rotate][i];
        block->raw32[i] = BitExtend[pat]*(block->color+1);
    }
    return 1;
}

int     DropBlock(BlockDesc* block)
{
    int bx = block->x;
    //int by = block->y + MATRIX_START;
    unsigned int br = block->rotate;
    int i;
    int full = 0;
    unsigned char iMat = curLine;
    unsigned char iNext;
    for(i=0;i<4;i++){
        unsigned long blockMap = block->patten[br][i];
        if(iMat==MATRIX_END)break;
        blockMap<<=(MAP_OFFSET+MAT_COL_CNT-bx-4);
        matrix[iMat].BitMap |= blockMap;
        if(matrix[iMat].BitMap == MAP_MASK && blockMap){
            full++;
            // Clear this line
            matrix[iMat].raw32[0] = 0;
            matrix[iMat].raw32[1] = 0;
            matrix[iMat].raw32[2] = 0;
            matrix[iMat].BitMap = MAP_MASK - (MAT_ROW_MASK<<MAP_OFFSET);

            // Remember next line
            iNext = matrix[iMat].nextLine;
            // Connect to next line
            matrix[matrix[iMat].preLine].nextLine = matrix[iMat].nextLine;
            matrix[iNext].preLine = matrix[iMat].preLine;

            // Move it to the first place
            matrix[iMat].preLine = matrix[firstLine].preLine;
            matrix[iMat].nextLine = firstLine;
            matrix[matrix[firstLine].preLine].nextLine = iMat;
            matrix[firstLine].preLine = iMat;
            firstLine = iMat;
            iMat = iNext;
        }else{
            unsigned long *p = (unsigned long *)(matrix[iMat].raw8 + bx);
            unsigned char pat = block->patten[block->rotate][i];
            *p |= BitExtend[pat]*(block->color+1);
            iMat = matrix[iMat].nextLine;
        }
    }
    //MATRIX_START = (MATRIX_START - full)&MATRIX_MASK;
    //MATRIX_END = (MATRIX_END - full)&MATRIX_MASK;
    return full;
}

void    DebugDump()
{
    int row;
    unsigned char iMat = matrix[firstLine].preLine;
    printf("***************Dump Start****************\n");
    for(row=-1;row<MAT_ROW_CNT+1;row++){
        int col;
        unsigned int tmp;
        for(col=0;col<MAT_COL_CNT;col++){
            unsigned char val = 0;
            if( (row >= curBlock.y&& row<curBlock.y+4) && (col >= curBlock.x && col < curBlock.x +4)){
                val = curBlock.BlockData[ (row - curBlock.y)*4 + col - curBlock.x ];
            }
            if(!val){
                val = matrix[iMat].Data[col];
            }
            if(row == -1 || row == MAT_ROW_CNT){
                if(!val){
                    printf("X");
                }else{
                    printf("%d",val);
                }
            }else{
                printf("%d",val);
            }
        }
        printf("   ");
        for(tmp=0x8000;tmp;tmp>>=1){
            printf(tmp&matrix[iMat].BitMap?"1":"0");
        }
        iMat = matrix[iMat].nextLine;
        printf("\n");
    }
    printf("\n");
}

void ScoreUp(int line)
{
    // 1 1
    // 2 3
    // 3 6
    // 4 12
    if(line){
        score += line*line - line/2;
        level = score/100;
        if(level>99){
            level = 99;
        }
        if(score > 99999){
            score = 99999;
        }
        DisplayScoreLevel();
    }
}
#define     NUM_0_BLOCK_A       20
#define     NUM_0_BLOCK_B       30
#define     GAME_OVER_BLOCK_A   40
#define     GAME_OVER_BLOCK_B   48
#define     PAUSE_BLOCK_A       56
#define     PAUSE_BLOCK_B       62

#define     GAME_OVER_LEN       8
#define     GAME_OVER_A         154
#define     GAME_OVER_B         173

#define     PAUSE_LEN           6
#define     PAUSE_A             231
#define     PAUSE_B             250

#define     LEVEL_10_A      337
#define     LEVEL_10_B      356 
#define     LEVEL_1_A       338
#define     LEVEL_1_B       357

#define     SCORE_10000_A   240
#define     SCORE_10000_B   259

void    DisplayScoreLevel(void)
{
    int num = level/10;
    unsigned char* scoreA = &scrBuf[SCORE_10000_A];
    unsigned char* scoreB = &scrBuf[SCORE_10000_B];
    unsigned int tpScore = score;
    scrBuf[LEVEL_10_A] = NUM_0_BLOCK_A + num;
    scrBuf[LEVEL_10_B] = NUM_0_BLOCK_B + num;
    num = level - num*10;
    scrBuf[LEVEL_1_A] = NUM_0_BLOCK_A + num;
    scrBuf[LEVEL_1_B] = NUM_0_BLOCK_B + num;

    if(score >9999){
        num = tpScore/10000;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;

        tpScore = tpScore - num*10000;
        num = tpScore/1000;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;
        
        tpScore = tpScore - num*1000;
        num = tpScore/100;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;

        tpScore = tpScore - num*100;
        num = tpScore/10;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;

        tpScore = tpScore - num*10;
        num = tpScore/1;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;
    }else if(score>999){
        *scoreA++ = 0;
        *scoreB++ = 0;

        num = tpScore/1000;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;
        
        tpScore = tpScore - num*1000;
        num = tpScore/100;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;

        tpScore = tpScore - num*100;
        num = tpScore/10;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;

        tpScore = tpScore - num*10;
        num = tpScore/1;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;
    }else if(score>99){
        *scoreA++ = 0;
        *scoreB++ = 0;
        *scoreA++ = 0;
        *scoreB++ = 0;

        num = tpScore/100;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;

        tpScore = tpScore - num*100;
        num = tpScore/10;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;

        tpScore = tpScore - num*10;
        num = tpScore/1;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;
    }else{  // less than 99
        *scoreA++ = 0;
        *scoreB++ = 0;
        *scoreA++ = 0;
        *scoreB++ = 0;

        num = tpScore/10;
        if(num){
            *scoreA++ = num + NUM_0_BLOCK_A;
            *scoreB++ = num + NUM_0_BLOCK_B;
        }else{
            *scoreA++ = 0;
            *scoreB++ = 0;

        }

        tpScore = tpScore - num*10;
        num = tpScore/1;
        *scoreA++ = num + NUM_0_BLOCK_A;
        *scoreB++ = num + NUM_0_BLOCK_B;

        *scoreA++ = 0;
        *scoreB++ = 0;
    }
}

void    DisplayGameOver()
{
    int i;
    for(i=0;i<GAME_OVER_LEN;i++){
        scrBuf[GAME_OVER_A+i] = GAME_OVER_BLOCK_A + i;
        scrBuf[GAME_OVER_B+i] = GAME_OVER_BLOCK_B + i;
    }
}

void    DisplayGamePause()
{
    int i;
    for(i=0;i<PAUSE_LEN;i++){
        scrBuf[PAUSE_A+i] = PAUSE_BLOCK_A + i;
        scrBuf[PAUSE_B+i] = PAUSE_BLOCK_B + i;
    }
}