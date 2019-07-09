int IsPalindrome(const char *str)

1. get a constant string;
2. set index with value 0;
3. set two pointers one at the beginning of the string and the other to the end of the string (with sizeof);
4. compare beginning to end until index become sizeof(string)/2,
   return 0 if not equal.
5. point adress up in pointer beginning and point adress down in pointer end;
6. return 1; 

void RmSpaces(char *str)

1. get a string;
2. declare two char[] variables: space = " " and tab "  ";
3. set a token with space and tab using strtok;
4. while there are tokens: print the token and whitespace and then keep looking in the string with strtok(NULL);




  


