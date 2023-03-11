# Stable Matching Problem
This project allocates visitors to bicycles according to people's favorite bicycles and the distance to them. The solution uses a breadth-first search to calculate the distances and the Gale-Shapley algorith to perform stable matchings.

## Usage
The input for the problem is a map and a preference list. People have their favorite bicycles and bicycles have the preference list based on the distance. The map has people, bicycles, obstacles and free way. The stable matching algorithm priorizes people's preference.

```
'*'    indicating a traversable section of the map. Visitors can transit through this region without problems.
'-'  indicating an obstacle on the map. Visitors cannot cross this region, they must go around it. 
Lowercase letters from ’a’ to  ’j’  ->  indicating a visitor, where visitor ’a’ has ID = 1, the visitor 'b' has ID = 2 and so on. Suppose a visitor can traverse spaces ocuppied by the others visitors.
Numbers, from '0' to '9'  ->  indicating a bicycle. Bike ’0’ has the highest priority (ID = 1), while bike ’9’ has the lowest priority (ID = 10). Suppose visitors can cross spaces occupied by a bicycle.
```
  
 The input has in the first line the number of bikes and visitors, in the second line the number of rows and columns of the map, the map in sequence and the next lines have a list of preferences of each visitor.
 
 ## Example
 ### input

>$2$
> 
>$4  4$
> 
>$a**0 $
>
>$--** $
>
>$*-** $
>
>b**1** 
>
>$3  4$ 
>
> $>4 5$

### output
> $ a 1$

> $ b 0

## Execution
Go to the directory where the program was stored
> cd -directory-

Now, compile and execute the program with an input file .txt
> g++ stable_matching.cpp

> ./a.out -teste_file.txt-

It will be displayed in the command line the generated pairs between bikes and people.
> 
> 
  
