#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "zlib.h"

#include <arpa/inet.h>

#define IHDR_STR "IHDR"
#define IDAT_STR "IDAT"
#define IEND_STR "IEND"

#define FILE_NAME "ByteByByte.png"

#define CODE_BUF_LEN 4000000
#define DECODE_BUF_LEN 15000000

typedef struct {
	uint32_t width;
	uint32_t height;
	uint8_t  bit_depth;
	uint8_t  color_type;
	uint8_t  compression_method;
	uint8_t  filter_method;
	uint8_t  intelace_method;
} png_ihdr_t;

typedef struct {
	uint8_t  red;
	uint8_t  green;
	uint8_t  blue;
	uint8_t  alfa;
}pixel_t;


void *myalloc(void *q, unsigned n, unsigned m)
{
    q = Z_NULL;
    return calloc(n, m);
}

void myfree(void *q, void *p)
{
    q = Z_NULL;
    free(p);
}

static alloc_func zalloc = myalloc;
static free_func zfree = myfree;

int uncompress_image(uint8_t *compr, uint32_t compr_len, uint8_t *uncompr, uint32_t uncompr_len, uint32_t *total_out)
{
    int err;
    z_stream d_stream; /* decompression stream */

    d_stream.zalloc = zalloc;
    d_stream.zfree = zfree;
    d_stream.opaque = (voidpf)0;

    d_stream.next_in  = compr;
    d_stream.avail_in = 0;
    d_stream.next_out = uncompr;

    printf("zstream format: %02x %02x\n", compr[0], compr[1]);

    err = inflateInit(&d_stream);
    if (err != Z_OK) return -1;

//    d_stream.avail_in = compr_len;
//    d_stream.avail_out = uncompr_len;


    while (d_stream.total_out < uncompr_len && d_stream.total_in < compr_len - 1)
    {
        d_stream.avail_in = d_stream.avail_out = 1;
        err = inflate(&d_stream, Z_NO_FLUSH);
        if (err == Z_STREAM_END) break;
        if (err != Z_OK)
        {
            printf("Total error(%d) in:%lu out:%lu\n", err, d_stream.total_in, d_stream.total_out);
            return -2;
        }
    }

    *total_out = d_stream.total_out;

    err = inflateEnd(&d_stream);
    if (err != Z_OK) return -3;

    return 0;
}

int find_chunk(int fd, char *chunk_name, uint32_t *chunk_len)
{
	char chunk_str[4];
	char len_arr[4] = {0};

	*chunk_len = 0;

	while (read(fd, chunk_str, 1) > 0)
	{
		if (chunk_str[0] == chunk_name[0])
		{
			read(fd, chunk_str + 1, sizeof(chunk_str) - 1);
			
			if (!memcmp(chunk_str, chunk_name, sizeof(chunk_str)))
			{
				return 0;
			}
		}

		len_arr[0] = len_arr[1];
		len_arr[1] = len_arr[2];
		len_arr[2] = len_arr[3];
		len_arr[3] = chunk_str[0];

		*chunk_len = ntohl(*((uint32_t *)len_arr));
	}

	return 1;
}

int find_code(int fd, uint32_t chunk_len)
{
	uint8_t bitmap[65535];
	int cur_len = 0, i;
	pixel_t pixel;
	int len_read = 0;

	while ((read(fd, &pixel, sizeof(pixel)) > 0) && (chunk_len - len_read) >= sizeof(pixel))
	{
		len_read += sizeof(pixel);

		if (((pixel.red & 0x01) == (pixel.green & 0x01)) && ((pixel.blue & 0x01) == (pixel.green & 0x01)))
		{
			bitmap[cur_len++] = pixel.red & 0x01;
		} else {
			if (cur_len >= 8)
			{
				for (i = 0; i < cur_len; i++)
				{
					printf("%d", bitmap[i]);
				}

				printf(" (%d)\n", cur_len);
			}

			cur_len = 0;
		}
	}

	return 0;
}

int find_code2(uint8_t *image_data, uint32_t size)
{
	uint8_t bitmap[65535];
	int cur_len = 0, i;
	pixel_t *pixel;
	int cur_pos = 0;

	while (cur_pos < size)
	{
        pixel = (pixel_t *)(image_data + cur_pos);

		if (((pixel->red & 0x01) == (pixel->green & 0x01)) && ((pixel->blue & 0x01) == (pixel->green & 0x01)))
		{
			bitmap[cur_len++] = pixel->red & 0x01;
		} else {
			if (cur_len > 64 && !(cur_len % 8))
			{
				for (i = 0; i < cur_len; i++)
				{
					printf("%d", bitmap[i]);
				}

				printf(" (%d)\n", cur_len);
			}

			cur_len = 0;
		}

        cur_pos += sizeof(pixel_t);
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	int fd = open(FILE_NAME, O_RDONLY);
	png_ihdr_t ihdr = {0};
	uint32_t chunk_len;
    uint8_t *coded_data = NULL, *decoded_data = NULL;
    uint32_t coded_len = 0, decoded_len = 0;
	int i = 0, res = 0;

	if (fd < 0)
	{
		perror("Failed to open file");
		return -1;
	}

	if (find_chunk(fd, IHDR_STR, &chunk_len))
	{
		printf("Faile to find chunk '%s'\n", IHDR_STR);

		goto _exit_close;
	}

	read(fd, &ihdr, sizeof(ihdr));

	ihdr.width  = ntohl(ihdr.width);
	ihdr.height = ntohl(ihdr.height);

	printf("IHDR:                   \n"
		   "\twidth:              %d\n"
		   "\theight:             %d\n"
		   "\tbit_depth:          %d\n"
		   "\tcolor_type:         %d\n"
		   "\tcompression_method: %d\n"
		   "\tfilter_method:      %d\n"
		   "\tintelace_method:    %d\n\n",
		   ihdr.width,
		   ihdr.height,
		   ihdr.bit_depth,
		   ihdr.color_type,
		   ihdr.compression_method,
		   ihdr.filter_method,
		   ihdr.intelace_method);


	// if (find_chunk(fd, IDAT_STR, &chunk_len))
	// {
	// 	printf("Faile to find chunk '%s'\n", IDAT_STR);

	// 	goto _exit_close;
	// }

    coded_data = malloc(CODE_BUF_LEN);
    decoded_data = malloc(DECODE_BUF_LEN);

	for (i = 0; !find_chunk(fd, IDAT_STR, &chunk_len); i++)
	{
		printf("%02d) IDAT len: %u\n", i, chunk_len);
		
//		find_code(fd, chunk_len);

        coded_len += read(fd, coded_data + coded_len, chunk_len);
	}

    printf("total len: %u\n", coded_len);

    if ((res = uncompress_image(coded_data, coded_len, decoded_data, DECODE_BUF_LEN, &decoded_len)))
    {
        printf("Uncopress error: %d\n", res);
        goto _exit_close;
    }

    find_code2(decoded_data, decoded_len);

_exit_close:

    free(coded_data);
    free(decoded_data);

	close(fd);

	return 0;
}

























