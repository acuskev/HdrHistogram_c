/**
 * yb_test.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#include <hdr/hdr_histogram.h>
#include <hdr/hdr_histogram_log.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


int main()
{
    //srand(time(NULL));
    // int i, value;
    struct hdr_histogram* histogram = (struct hdr_histogram*) calloc(1, sizeof(struct hdr_histogram) + 176*4);

    hdr_init(1, 16777215, 32, histogram);
    printf("subbucket count: %d \n", histogram->sub_bucket_count);
    printf("bucket count: %d \n", histogram->bucket_count);

    // for (i = 0; i < 20; i++)
    // {
    //     value = rand() % 9000000 + 1;
    //     if (value >= 4194304 & value < 8388608) {
    //         printf("Added value: %d \n", value);
    //     }
    //     if (value >= 8388608) {
    //         printf("Added big value: %d \n", value);
    //     }
    //     // printf("Added value: %d \n", value);
    //     hdr_record_value(histogram, value);
    // }
    // hdr_record_value(histogram, 1);
    // hdr_record_value(histogram, 2);
    // hdr_record_value(histogram, 3);
    // hdr_record_value(histogram, 4);
    // hdr_record_value(histogram, 4);
    // hdr_record_value(histogram, 31);
    // hdr_record_value(histogram, 32);
    // hdr_record_value(histogram, 33);
    // hdr_record_value(histogram, 8388607);
    // hdr_record_value(histogram, 8388608);
    // hdr_record_value(histogram, 9000000);
    // hdr_record_value(histogram, 16777215);
    // hdr_record_values(histogram, 1, 13);
    hdr_record_values(histogram, 1, 4);
    // hdr_record_values(histogram, 0, 4);
    hdr_record_value(histogram, 2);
    hdr_record_value(histogram, 8);
    // hdr_record_values(histogram, 11, 3);
    // hdr_record_values(histogram, 15, 1);
    // hdr_record_values(histogram, 20, 4);
    // hdr_record_value(histogram, 32);

    printf("total number of subbuckets: %d \n", histogram->counts_len);

    int mem = hdr_get_memory_size(histogram);
    // int mem = sizeof(histogram->counts);
    printf("Footprint: histogram struct size: %lu, total size: %d \n", sizeof(struct hdr_histogram), mem);

    // printf("\nPercentiles Printing\n\n");
    // hdr_percentiles_print(histogram,stdout,5,1.0);
    printf("\nPrinting \n\n");
    struct hdr_iter iter;
    char * result;
    result = malloc(2000);

    // // logarithmic iterator
    // int64_t total_count = 0;
    // hdr_iter_log_init(&iter, histogram, 1,2);

    // while(hdr_iter_next(&iter))
    // {
    //     if (iter.specifics.log.count_added_in_this_iteration_step >= 0)
    //     {
    //         char buf[100];
    //         total_count = iter.specifics.log.count_added_in_this_iteration_step;
    //         if (iter.counts_index >= histogram->counts_len)
    //         {
    //             printf("logging bucket for interval: %lld-%lld, count_added: %lld \n", iter.value_iterated_to, iter.specifics.log.next_value_reporting_level, iter.specifics.log.count_added_in_this_iteration_step);
    //             snprintf(buf, 100,"%lld-%lld: %lld", iter.value_iterated_to, iter.specifics.log.next_value_reporting_level, total_count);
    //         }
    //         else if (iter.value_iterated_to == 1 & iter.value_iterated_from == 0)
    //         {
    //             printf("logging bucket for: %lld, count_added: %lld \n", iter.value_iterated_to, iter.specifics.log.count_added_in_this_iteration_step);
    //             snprintf(buf, 100,"%lld: %lld, ",iter.value_iterated_to, total_count);
    //         }
    //         else
    //         {
    //             printf("logging bucket for interval: %lld-%lld, count_added: %lld \n", iter.value_iterated_from, iter.value_iterated_to, iter.specifics.log.count_added_in_this_iteration_step);
    //             snprintf(buf, 100,"%lld-%lld: %lld, ",iter.value_iterated_from, iter.value_iterated_to, total_count);
    //         }
    //         strncat(result, buf, strlen(buf));
    //     }
    // }
    // printf("ending highest trackable value: %lld \n", histogram->highest_trackable_value);
    // printf("%s \n", result);

    // normal iterator
    hdr_iter_init(&iter, histogram);
    while(hdr_iter_next(&iter))
    {
        char buf[100];
        snprintf(buf, 100,"index: %d, [%lld-%lld), count: %d \n",iter.counts_index, iter.value_iterated_to, iter.highest_equivalent_value + 1, iter.count);
        strncat(result, buf, strlen(buf));
    }
    printf("%s \n", result);

    int64_t p90 = hdr_value_at_percentile(histogram, 90);
    int64_t p50 = hdr_value_at_percentile(histogram, 50);
    int64_t p99 = hdr_value_at_percentile(histogram, 99);

    printf("p99: %lld, p90: %lld, p50: %lld \n", p99, p90, p50);

    printf("max value: %lld, highest_trackable_value: %lld \n", histogram->max_value, histogram->highest_trackable_value);
    int derived_max_mag = histogram->sub_bucket_half_count_magnitude + 1 + histogram->bucket_count - 1;
    int derived_max = pow(2, derived_max_mag);
    printf("derived max: %d \n", derived_max);

    return 0;
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
