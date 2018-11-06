#include <stdio.h>
#include <unistd.h>
#include <iwlib.h>
#include<string>
#include<iostream>
using namespace std;
static char *NetCardID = strdup("wlp5s0");
void do_scan(int sock, iwrange *range)
{
    wireless_scan_head head;
    wireless_scan *result;


    /* Perform the scan */
    if (iw_scan(sock,NetCardID, range->we_version_compiled, &head) < 0) {
        printf("Error during iw_scan. Aborting.\n");
        return;
    }

    /* Traverse the results */
    result = head.result;
    while (result != nullptr) {
        string ESSID=result->b.essid;
        cout<<ESSID<<endl;

        if (result->b.has_freq) {
            double freq=result->b.freq/pow(10,9);
            cout<<freq<<"GHz"<<endl;

        }

        if (result->has_stats) {
            int level=result->stats.qual.level- 0x100;
            cout<<level<<"dB"<<endl;

        }
        printf("------------------\n");
        result = result->next;
    }

    result = head.result;
    /* free results */
    while (result && result->next) {
        wireless_scan *tmp;
        tmp = result->next;
        free (result);
        result = tmp;
    }
    if (result) {
        free(result);
    }
}

int main(void)
{

    iwrange range;
    int sock, i;

    sock = iw_sockets_open();

    /* Get some metadata to use for scanning */
    if (iw_get_range_info(sock, NetCardID, &range) < 0) {
        printf("Error during iw_get_range_info. Aborting.\n");
        return -1;
    }

    for (i=0; i<5; i++) {
        printf("\033[2J");
        do_scan(sock, &range);
        sleep(1);
    }

    iw_sockets_close(sock);
    return 0;
}
