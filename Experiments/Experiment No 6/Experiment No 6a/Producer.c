#include <stdio.h>
#include "abstract.h"
int main()
{
init_shm();
printf("Press Enter to produce, e to exit: ");
while(getchar()!='e')
{
producer();
}
close_shm();
return 0;
}
