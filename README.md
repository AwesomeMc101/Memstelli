# PLEASE READ OVER ORIGINAL CONSTELLI SYNTAX

### You can find the original tutorial on programming Constelli in the readme here.
https://github.com/AwesomeMc101/Constelli


# EDITING MEMORY IN MEMSTELLI

### Video link: https://www.youtube.com/watch?v=45qdE_o4xK0

* As shown in the video, this tutorial will use the example of a process
called "fake game.exe" and writing to and reading from a player's gold.

* The game initially prints out the memory address the gold is stored at,
saving us the time of using Cheat Engine to find it.

### To open the process, we write this code.
```
openprocess("fake game.exe");
```
* You can only have `1` global handle (process) opened at one time.

Now, let's read the memory initially stored as our gold.
The game told us the address was at `00F7FCBC`. So, we can read it doing this:
```
readmemory("00F7FCBC");
```

To write memory, it's the same with another argument.
### Lets set our gold to 1 million.
```
writememory("00F7FCBC", 1000000);
```

Lastly, we close our process simply calling:
```
closeprocess;
```
