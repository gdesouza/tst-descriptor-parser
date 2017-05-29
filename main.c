#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


struct ts_descr_t {
    uint8_t   commandByte;
    uint8_t   length;
    uint16_t  addressMode;
    //uint8_t   downloadMode;
    uint8_t   zoneId[6];
    uint16_t  phaseDownload;
    uint8_t   manufacturerId[6];
    uint16_t  boxListmode;
    uint8_t   numberUb;
    uint8_t   dataSize;
    uint8_t   checksum;
    uint8_t   filler;
};


void print_buffer(void* buffer, size_t size)
{
    unsigned char *c_buffer = (char*) buffer;
    int i = 0;
    for (i=0; i<size; i++) {
        printf("%02X ", c_buffer[i]);
        if (i%10==9) printf("\n");
    }
    printf("\n");
}



void exec(const char* input_filename, const char* output_dir) {
    // Read input file
    FILE *f_handle = fopen(input_filename, "r");
    if (NULL == f_handle) {
        printf("Unable to open file %s\n", input_filename);
        exit(0);
    }

    // copy contents of file to memory
    unsigned char buffer[256];
    fread(buffer, sizeof(buffer), 1, f_handle);

    // close input file
    close(f_handle);

    // ==================================================================
    // parse descriptor
    // ==================================================================
    struct ts_descr_t ts_descriptor;
    memcpy(&ts_descriptor, buffer, sizeof(ts_descriptor)-1);

    printf("Descriptor:\n");
    print_buffer(&ts_descriptor, sizeof(ts_descriptor)-1);
    
    // ==================================================================
    // parse payload
    // ==================================================================
    char* payload = malloc(ts_descriptor.numberUb*ts_descriptor.dataSize+2);
    memcpy(payload, buffer+sizeof(ts_descriptor)-1, ts_descriptor.numberUb*ts_descriptor.dataSize+2);
    printf("Payload (%d bytes):\n", ts_descriptor.numberUb*ts_descriptor.dataSize+2);
    print_buffer(payload, ts_descriptor.numberUb*ts_descriptor.dataSize+2);

    

    

    free(payload);
}

















/* Le nom du program */
const char* program_name;

/* Prints usage information for this program to STREAM (typically
   stdout or stderr), and exit the program with EXIT_CODE. Does not
   return. */

void print_usage (FILE* stream, int exit_code)
{
    fprintf (stream, "Usage: %s options -i inputfile -o output_directory\n", program_name);
    fprintf (stream,
            "  -h  --help             Display this usage information.\n"
            "  -i  --input filename   Read input from file.\n"
            "  -o  --output directory Output directory.\n"
            "  -v  --verbose          Print verbore messages.\n");
    exit (exit_code);
}

int main (int argc, char* argv[])
{
    int next_option;

    /* A string listing valid short options letters. */
    const char* const short_options = "hi:v";

    /* An array listing valid long options. */
    const struct option long_options[] = {
        { "help",    0, NULL, 'h' },
        { "output",  1, NULL, 'o' },
        { "input",   1, NULL, 'i' },
        { "verbose", 0, NULL, 'v' },
        { NULL,      0, NULL,  0  }  /* Required at end of array. */
    };

    /* The name of the file to receive program output, or NULL for 
       standard output. */
    const char* output_dir = NULL;

    /* The name of the input file containing the descriptor */
   const char* input_filename = NULL;

    /* Whether to display verbose messages. */
    int verbose = 0;

    /* Remember the name of the program to incorporate in messages.
       The name is stored in argv[0] */
    program_name = argv[0];

    do {
        next_option = getopt_long (argc, argv, short_options, 
                                    long_options, NULL);

        switch (next_option)
        {
        case 'h':        /* - h  or -help */
            /* User has requestedusagem information. Print it to standard
                output, and exi with exit code ziro (normal termination) */
            print_usage (stdout, 0);

        case 'o':       /* -o or --output */
            /* This option takes an argument, the name of the output file. */
            output_dir = optarg;
            break;

        case 'i':       /* -o or --output */
            /* This option takes an argument, the name of the output file. */
            input_filename = optarg;
            break;            
        
        case 'v':       /* -v or --verbose */
            verbose = 1;
            break;
        
        case '?':       /* The user specified an invalid option */
            /* Print usage information to standard error, and exit with exit
                code one (indicating abnormal termination).  */
            print_usage(stderr, 1);

        case -1:        /* Done with options */
            break;

        default:        /* Something else: unexpected. */
            abort ();
        }
    }
    while (next_option != -1);



    /* The main program goes here. */
    exec(input_filename, output_dir);


    return 0;

}