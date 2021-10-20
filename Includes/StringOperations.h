#ifndef INCLUDED_SO_H
#define INCLUDED_SO_H

#include <string>

using namespace std;

//removes [SPACE] characters from the end of string argumented
string removeSpacesFromEnd(string str)
{
    int lC;
    for (lC = str.length() - 1; lC >= 0; --lC)
        if (str[lC] != ' ')
            break;
    str.resize(lC + 1);
    return str;
}

//appends n [SPACE] characters to the end of string argumented
string appendSpaces(string str, int n)
{
    for(int i = 0; i < n; ++i)
        str += " ";
    return str;
}

//returns whether strings argumented s1 and s2 are same or not
bool compareStrings(string s1, string s2)
{
    if (s1.length() == s2.length())
    {
        for (int i = 0; i < s1.length(); ++i)
            if (s1[i] != s2[i])
                return false;
        return true;
    }
    return false;
}

#endif
