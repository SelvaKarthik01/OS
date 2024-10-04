#include <stdio.h>
#include "abstract.h"
int main()
{
init_shm();
printf("Press Enter to Consume, e to exit:");
while (getchar()!='e')
{
consumer();
}
close_shm();
return 0;
}
