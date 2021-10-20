#ifndef INCLUDED_CI_H
#define INCLUDED_CI_H

//returns the index (integer value) corresponding to the argumented character 
int getIndexFromCharacter(char c)
{
    int ci = int(c);
    if(ci >= 65 && ci <= 90)                //A-Z
        return ci - 65;
    else if(ci >= 97 && ci <= 122)          //a-z
        return ci - 71;
    else if(ci >= 48 && ci <= 57)           //0-9
        return ci + 4;
    else if(ci >= 32 && ci <= 33)           //special characters
        return ci + 30;
    else if(ci >= 35 && ci <= 47)           //special characters
        return ci + 29;
    else if(ci >= 58 && ci <= 64)           //special characters
        return ci + 19;
    else if(ci == 91)                       //special characters
        return ci - 7;
    else if(ci >= 93 && ci <= 96)           //special characters
        return ci - 8;
    else if(ci >= 123 && ci <= 126)         //special characters
        return ci - 34;
}

//returns the character corresponding to the argumented index 
char getCharacterFromIndex(int i)
{
    if(i >= 0 && i <= 25)                   //A-Z
        return char(i + 65);
    else if(i >= 26 && i <= 51)             //a-z
        return char(i + 71);
    else if(i >= 52 && i <= 61)             //0-9
        return char(i - 4);
    else if(i >= 62 && i <= 63)             //special characters
        return char(i - 30);
    else if(i >= 64 && i <= 76)             //special characters
        return char(i - 29);
    else if(i >= 77 && i <= 83)             //special characters
        return char(i - 19);
    else if(i == 84)                        //special characters
        return char(i + 7);
    else if(i >= 85 && i <= 88)             //special characters
        return char(i + 8);
    else if(i >= 89 && i <= 92)             //special characters
        return char(i + 34);
}

#endif

//Total number of characters: 93
//Sequence of characters with increasing value of index (starting from 0)
// A B C D E F G H I J K L M N O P Q R S T U V W X Y Z              0 - 25
// a b c d e f g h i j k l m n o p q r s t u v w x y z              26 - 51
// 0 1 2 3 4 5 6 7 8 9                                              52 - 61
// [SPACE]                                                          62
// !                                                                63
// #                                                                64
// $                                                                65
// %                                                                66
// &                                                                67
// '                                                                68
// (                                                                69
// )                                                                70
// *                                                                71
// +                                                                72
// ,                                                                73
// -                                                                74
// .                                                                75
// /                                                                76
// :                                                                77
// ;                                                                78
// <                                                                79
// =                                                                80
// >                                                                81
// ?                                                                82
// @                                                                83
// [                                                                84
// ]                                                                85
// ^                                                                86
// _                                                                87
// `                                                                88
// {                                                                89
// |                                                                90
// }                                                                91
// ~                                                                92
