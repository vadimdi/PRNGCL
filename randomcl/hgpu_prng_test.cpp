/******************************************************************************
 * @file     hgpu_prng_test.cpp
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.1.2
 *
 * @brief    [PRNGCL library]
 *           Pseudo-random number generators for HGPU package
 *           tests submodule
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013-2015 Vadim Demchik
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/

#include "hgpu_prng.h"

unsigned int
HGPU_PRNG_tests(HGPU_GPU_context* context,HGPU_parameter** parameters){
    unsigned int result = 0;

#ifdef PRNGCL_MD5_INIT
    // Toy PRNGs:
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_PM,      1,HGPU_precision_single,10000,0.83041679859161377);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_CONSTANT,1,HGPU_precision_single,10000,0.81189441680908203);

    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_XOR128,  1,HGPU_precision_single,1000000,0.076715946197509766);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_XOR7,    1,HGPU_precision_single,1000000,0.093575797975063324);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANECU,  1,HGPU_precision_single,1000000,0.96220004558563232);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANMAR,  1,HGPU_precision_single,1000000,0.54384857416152954);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANLUX,  1,HGPU_precision_single,1000000,0.085599660873413086);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_MRG32K3A,1,HGPU_precision_single,1000000,0.51180928945541382);


    // Toy PRNGs:
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_PM,      1,HGPU_precision_double,1000000,0.26611024494251845);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_CONSTANT,1,HGPU_precision_double,1000000,0.81189443287439644);

    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_XOR128,  1,HGPU_precision_double,1000000,0.79761336774107472);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_XOR7,    1,HGPU_precision_double,1000000,0.27669601110197489);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANECU,  1,HGPU_precision_double,1000000,0.88385315708986256);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANMAR,  1,HGPU_precision_double,1000000,0.17660977581855250);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANLUX,  1,HGPU_precision_double,1000000,0.22363614294126266);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_MRG32K3A,1,HGPU_precision_double,1000000,0.73900908237277685);
#else
    // Toy PRNGs:
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_PM,      1,HGPU_precision_single,10000,0.8641089363322169);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_CONSTANT,1,HGPU_precision_single,10000,0.0234896258916706);

    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_XOR128,  1,HGPU_precision_single,1000000,0.6839658189564943);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_XOR7,    1,HGPU_precision_single,1000000,0.09432704793289304);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANECU,  1,HGPU_precision_single,1000000,0.1810733857564628);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANMAR,  1,HGPU_precision_single,1000000,0.9911311864852905);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANLUX,  1,HGPU_precision_single,1000000,0.42132478952407837);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_MRG32K3A,1,HGPU_precision_single,1000000,0.8171486894057429);


    // Toy PRNGs:
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_PM,      1,HGPU_precision_double,1000000,0.330385444271241);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_CONSTANT,1,HGPU_precision_double,1000000,0.023489625891670585);

    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_XOR128,  1,HGPU_precision_double,1000000,0.75871549689209505);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_XOR7,    1,HGPU_precision_double,1000000,0.75934357677468545);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANECU,  1,HGPU_precision_double,1000000,0.31995889215386432);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANMAR,  1,HGPU_precision_double,1000000,0.6419413344753121);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_RANLUX,  1,HGPU_precision_double,1000000,0.16665428407858204);
    result += HGPU_PRNG_test(context,parameters,HGPU_PRNG_MRG32K3A,1,HGPU_precision_double,1000000,0.0075525209409365449);
#endif

    printf(" **************************************************\n");
    if (result)
        printf(" %u test(s) failed!!!\n",result);
    else
        printf(" All tests passed\n");
    printf(" **************************************************\n");

    return result;
}


void
HGPU_PRNG_benchmarks(HGPU_GPU_context* context,HGPU_parameter** parameters){
    double result = 0.0;

    char* timestamp = HGPU_timer_get_current_datetime();
        printf("\nStarted at: %s (%u tics/sec)\n\n",timestamp,(unsigned int) CLOCKS_PER_SEC);
    free(timestamp);

    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_CONSTANT,HGPU_precision_single);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_PM,      HGPU_precision_single);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_XOR128,  HGPU_precision_single);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_XOR7,    HGPU_precision_single);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_RANECU,  HGPU_precision_single);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_RANMAR,  HGPU_precision_single);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_RANLUX,  HGPU_precision_single);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_MRG32K3A,HGPU_precision_single);

    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_CONSTANT,HGPU_precision_double);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_PM,      HGPU_precision_double);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_XOR128,  HGPU_precision_double);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_XOR7,    HGPU_precision_double);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_RANECU,  HGPU_precision_double);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_RANMAR,  HGPU_precision_double);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_RANLUX,  HGPU_precision_double);
    result += HGPU_PRNG_benchmark(context,parameters,HGPU_PRNG_MRG32K3A,HGPU_precision_double);

    timestamp = HGPU_timer_get_current_datetime();
        printf("\nFinished at: %s\n",timestamp);
    free(timestamp);

}
