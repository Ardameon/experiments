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
#define ELTEX_EXTENSION_SIZE_MAX 128

typedef enum {
    eELTEX_WAV_IE_IS_READ = 34
} eltex_wav_ie_type_e;

typedef struct {
    FILE *f;
    uint32_t extension_offset;
} stWAVExtended;

typedef struct {
    char     name[5];
    uint32_t extension_size;

    uint8_t  data[0];
}__attribute__((packed)) stELTEXExtension;

typedef struct {
    uint16_t type;
    uint32_t size;
}__attribute__((packed)) stELTEXWAVIEHdr;

typedef struct {
    stELTEXWAVIEHdr hdr;
    uint8_t file_is_read;
}__attribute__((packed)) stIEFileRead;

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


int wav_open(char *file_name, stWAVExtended *wav_file)
{
    FILE *f = fopen(file_name, "r+");
    size_t pos;
    stWAVHeader hdr;
    int res = -1;

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
        goto _exit;
    }

    fseek(f, 0L, SEEK_SET);

    fread(&hdr, 1, sizeof(hdr), f);

    if (strncmp(hdr.RIFF.name, "RIFF", 4) || strncmp(hdr.WAVE.name, "WAVE", 4))
    {
        printf("File '%s' is not in WAV format\n", file_name);
        goto _exit;
    }

    fseek(f, 0L, SEEK_SET);

    wav_file->f = f;
    wav_file->extension_offset = hdr.RIFF.file_size + 8;
    res = 0;

_exit:
    return res;
}


int wav_eltex_extesion_read(stWAVExtended *wav_file, stELTEXExtension *ext)
{
    int res = -1;
    size_t len;

    fseek(wav_file->f, wav_file->extension_offset, SEEK_SET);

    len = fread(ext, 1, sizeof(*ext), wav_file->f);

    if (len != sizeof(*ext))
    {
        printf("Can't get eltex extension from file: wrong size: %zd, expected: %ld\n", len, sizeof(*ext));
        goto _exit;
    }

    if (strncmp(ext->name, ELTEX_EXTENSION_STR, strlen(ELTEX_EXTENSION_STR)))
    {
        printf("Extension is not ELTEX: '%.5s'\n", ext->name);
        goto _exit;
    }

    res = 0;

_exit:
    fseek(wav_file->f, 0L, SEEK_SET);

    return res;
}


int wav_eltex_extesion_write(stWAVExtended *wav_file, stELTEXExtension *ext)
{
    fseek(wav_file->f, wav_file->extension_offset, SEEK_SET);

    fwrite(ext, 1, sizeof(*ext), wav_file->f);

    fseek(wav_file->f, 0L, SEEK_SET);

    return 0;
}


long wav_eltex_ie_find(stWAVExtended *wav_file, eltex_wav_ie_type_e type, int ie_size)
{
    long res = -1, ret;
    stELTEXExtension ext = {0};
    uint8_t buff[ELTEX_EXTENSION_SIZE_MAX];
    stELTEXWAVIEHdr *ie_hdr;
    int buff_offset = 0;

    ret = wav_eltex_extesion_read(wav_file, &ext);
    if (ret)
    {
        printf("Can't get IE: no extension\n");
        goto _exit;
    }

    if (ext.extension_size > ELTEX_EXTENSION_SIZE_MAX)
    {
        printf("Extension size is not valid: %u expected max: %d\n",
                ext.extension_size, ELTEX_EXTENSION_SIZE_MAX);
        goto _exit;
    }

    fseek(wav_file->f, wav_file->extension_offset + sizeof(ext), SEEK_SET);

    fread(buff, 1, ext.extension_size, wav_file->f);

    while (buff_offset < ext.extension_size)
    {
        ie_hdr = (stELTEXWAVIEHdr *)(buff + buff_offset);

        if ((ie_hdr->type == type) && (ie_hdr->size == (ie_size - sizeof(*ie_hdr))))
        {
            printf("IE found\n");
            res = wav_file->extension_offset + sizeof(ext) + buff_offset;
            goto _exit;
        }

        buff_offset += (ie_hdr->size + sizeof(*ie_hdr));
    }

    printf("IE not found\n");
_exit:
    fseek(wav_file->f, 0L, SEEK_SET);

    return res;
}


int wav_eltex_ie_read(stWAVExtended *wav_file, eltex_wav_ie_type_e type, void *ie_ptr, int ie_size)
{
    int res = -1;
    long ie_offset;

    ie_offset = wav_eltex_ie_find(wav_file, type, ie_size);
    if (ie_offset < 0)
    {
        printf("Can't find ie in file\n");
        goto _exit;
    }

    fseek(wav_file->f, ie_offset, SEEK_SET);

    fread(ie_ptr, 1, ie_size, wav_file->f);
    res = 0;

_exit:
    fseek(wav_file->f, 0L, SEEK_SET);

    return res;
}


int wav_eltex_ie_write(stWAVExtended *wav_file, eltex_wav_ie_type_e type, void *ie_ptr, int ie_size)
{
    int res = -1, ret;
    size_t len = 0;
    stELTEXExtension ext = {0};
    int ie_offset = wav_file->extension_offset + sizeof(ext);
    uint8_t ie_exists = 0;

    ret = wav_eltex_extesion_read(wav_file, &ext);
    if (ret)
    {
        printf("Can't get IE: no extension add new\n");

        memcpy(ext.name, ELTEX_EXTENSION_STR, strlen(ELTEX_EXTENSION_STR));
        ext.extension_size = 0;

        wav_eltex_extesion_write(wav_file, &ext);
    }
    else
    {
        long offset = wav_eltex_ie_find(wav_file, type, ie_size);
        if (offset >= 0)
        {
            ie_exists = 1;
            ie_offset = offset;
        }
    }

    fseek(wav_file->f, ie_offset, SEEK_SET);

    len = fwrite(ie_ptr, 1, ie_size, wav_file->f);
    if (len != ie_size)
    {
        printf("Can't write IE: bytes written: %zd expected: %d\n", len, ie_size);
        goto _exit;
    }

    if (!ie_exists)
    {
        ext.extension_size += ie_size;
        wav_eltex_extesion_write(wav_file, &ext);
    }

    res = 0;

_exit:
    fseek(wav_file->f, 0L, SEEK_SET);

    return res;
}


void wav_mark(stWAVExtended *wav_file, uint8_t mark_as_read)
{
    stIEFileRead ie;
    int res;

    ie.hdr.type = eELTEX_WAV_IE_IS_READ;
    ie.hdr.size = sizeof(ie.file_is_read);
    ie.file_is_read = !!mark_as_read;

    res = wav_eltex_ie_write(wav_file, eELTEX_WAV_IE_IS_READ, &ie, sizeof(ie));
    if (!res)
        printf("File marked as %s\n", mark_as_read ? "READ" : "UNREAD");
}


void wav_check(stWAVExtended *wav_file)
{
    stIEFileRead ie;
    int res = -1;
    uint8_t marked_as_read = 0;

    res = wav_eltex_ie_read(wav_file, eELTEX_WAV_IE_IS_READ, &ie, sizeof(ie));
    if (!res)
    {
        marked_as_read = ie.file_is_read;
    }

    printf("File %s\n", marked_as_read ? "READ" : "UNREAD");
}


int main(int argc, char **argv)
{
    char cmd[64];
    stWAVExtended wav_file;
    int res;

    if (argc < 2)
    {
        printf("Not enough arguments\n");
        goto _exit;
    }

    res = wav_open(argv[1], &wav_file);
    if (res)
        goto _exit;

    print_prompt();

    while(fgets(cmd, sizeof(cmd), stdin))
    {
        switch (*cmd)
        {
            case 'r':
                wav_mark(&wav_file, 1);
                break;

            case 'u':
                wav_mark(&wav_file, 0);
                break;

            case 'q':
                goto _exit;
                break;

            case 'c':
                wav_check(&wav_file);
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
