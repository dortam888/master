int IsPalindrome(const char *str)

1. get a constant string;
2. set two pointers one at the beginning of the string and the other to the end of the string (with strlen);
3. compare beginning to end as long as end is bigger than beginning,
   return 0 if not equal.
4. increase by 1 pointer beginning and decrease by 1 pointer end;
5. return 1; 

void SevenBoom(int from, int to)

1. get two integers from and "to";
2. until from is equal to to increase it by 1;
3. check if from is divided by 7 (with %7) or if 7 is in the number with function:
3.1 . check if number%10 == 7 and if not divide it by 10 and check again until it's 0;
4. print "BOOM" if (3) is true and print number if (3) is false;  


void RmSpaces(char *str)

1. get a string;
2. declare two char[] variables: space = " " and tab "  ";
3. set a token with space and tab using strtok;
4. while there are tokens: print the token and whitespace and then keep looking in the string with strtok(NULL);


void AddStrings(const char *, const char *)

1. get 2 strings with numbers 
2. 


  


