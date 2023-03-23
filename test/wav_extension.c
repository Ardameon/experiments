#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
	struct {
		char name[4];
		uint32_t file_size;
	} RIFF;

	struct {
		char name[4];
	} WAVE;

	struct {
		char name[4];
		uint32_t fmt_size;
		uint16_t fmt_compression_code;
		uint16_t fmt_number_of_channels;
		uint32_t fmt_sample_rate;
		uint32_t fmt_avg_bytes_per_sec;
		uint16_t fmt_block_align;
		uint16_t fmt_bits_per_sample;
		uint16_t fmt_extra_fmt_bytes;
	} __attribute__((packed)) fmt_;

	struct {
		char name[4];
		uint32_t fact_size;
		uint32_t fact_samples_count;
	}fact;

	struct {
		char name[4];
		uint32_t data_size;
	} data;

}__attribute__((packed)) stWAVHeader;

#define ELTEX_EXTENSION_STR "ELTEX"

typedef struct {
    char     name[5];
    uint32_t extension_size;

    uint8_t  data[0];
}__attribute__((packed)) stELTEXextension;


void print_prompt(void)
{
    printf("enter command > ");
}


void print_help(void)
{
    printf(
            "r - mark file as READ\n"
            "u - mark file as UNREAD\n"
            "c - check file status\n"
            "h - print this help\n"
            "q - quit\n"
            );
}


FILE *wav_open(char *file_name)
{
    FILE *f = fopen(file_name, "r+");
    size_t pos;
    stWAVHeader hdr;

    if (!f)
    {
        printf("Can't open file '%s'\n", file_name);
        goto _exit;
    }

    fseek(f, 0L, SEEK_END);
    pos = ftell(f);

    if (pos < sizeof(stWAVHeader))
    {
        printf("File size is less than WAV-header\n");
        f = NULL;
        goto _exit;
    }

    fseek(f, 0L, SEEK_SET);

    fread(&hdr, 1, sizeof(hdr), f);

    if (strncmp(hdr.RIFF.name, "RIFF", 4) || strncmp(hdr.WAVE.name, "WAVE", 4))
    {
        printf("File '%s' is not in WAV format\n", file_name);
        f = NULL;
        goto _exit;
    }

    fseek(f, 0L, SEEK_SET);

_exit:
    return f;
}


void wav_mark(FILE *f, uint8_t mark_as_read)
{
    stWAVHeader hdr = {0};
    uint8_t buff[sizeof(stELTEXextension) + 1] = {0};
    stELTEXextension *ext = (stELTEXextension *)buff;

    memcpy(ext->name, ELTEX_EXTENSION_STR, strlen(ELTEX_EXTENSION_STR));
    ext->extension_size = sizeof(mark_as_read);
    ext->data[0] = !!mark_as_read;

    fread(&hdr, 1, sizeof(hdr), f);

    fseek(f, hdr.RIFF.file_size + 8, SEEK_SET);

    fwrite(ext, 1, sizeof(buff), f);

    fseek(f, 0L, SEEK_SET);

    printf("File marked as %s\n", mark_as_read ? "READ" : "UNREAD");
}


void wav_check(FILE *f)
{
    stWAVHeader hdr;
    uint8_t buff[sizeof(stELTEXextension) + 1] = {0};
    stELTEXextension *ext;
    uint8_t marked_as_read = 0;

    fread(&hdr, 1, sizeof(hdr), f);

    fseek(f, hdr.RIFF.file_size + 8, SEEK_SET);

    fread(buff, 1, sizeof(buff), f);
    ext = (stELTEXextension *)buff;

    if (!strncmp(ext->name, ELTEX_EXTENSION_STR, strlen(ELTEX_EXTENSION_STR)))
        marked_as_read = ext->data[0];

    printf("File %s\n", marked_as_read ? "READ" : "UNREAD");

    fseek(f, 0L, SEEK_SET);
}


int main(int argc, char **argv)
{
    char cmd[64];
    FILE *f;

    if (argc < 2)
    {
        printf("Not enough arguments\n");
        goto _exit;
    }

    f = wav_open(argv[1]);
    if (!f)
        goto _exit;

    print_prompt();

    while(fgets(cmd, sizeof(cmd), stdin))
    {
        switch (*cmd)
        {
            case 'r':
                wav_mark(f, 1);
                break;

            case 'u':
                wav_mark(f, 0);
                break;

            case 'q':
                goto _exit;
                break;

            case 'c':
                wav_check(f);
                break;

            case 'h':
            default:
                print_help();
                break;
        }

        print_prompt();
    }

_exit:
    return 0;
}
