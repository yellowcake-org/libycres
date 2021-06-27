#include <stdio.h>
#include <libycres.h>

int main() {
    unsigned long dirs_count = 0;
    yc_res_dat_count(NULL, NULL, &dirs_count);
    
    printf("Dirs count: %zu.\n", dirs_count);    
    return 0;
}
