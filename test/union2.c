#include <stdio.h>
#include <stdint.h>

typedef struct {
	uint8_t obj_id;
	union
	{
		uint32_t ID;
		char name[4];
	} id_data;
	
} type_gen;

typedef struct {
	type_gen gen_info;

	uint32_t spec_info1;
	
} type_spec1;

typedef struct {
	type_gen gen_info;

	uint8_t spec_info_array[12];
	
} type_spec2;

typedef struct {
	uint8_t info_id;

	union
	{
		type_gen gen;
		type_spec1 spec1;
		type_spec2 spec2;
	} un;
	
} info;


int main(int argc, char const *argv[])
{
	info object;

	object.info_id = 1;
	object.un.gen.obj_id = 13;
	object.un.gen.id_data.ID = 1234567;

	object.un.spec1.spec_info1 = 256;

	sprintf(object.un.spec2.spec_info_array, "0987654321");

	printf("(gen)   object: info_id:%d obj_id:%d ID:%d\n", object.info_id, object.un.gen.obj_id, object.un.gen.id_data.ID);
	printf("(spec1) object: info_id:%d obj_id:%d ID:%d spec1:%d\n", object.info_id, object.un.spec1.gen_info.obj_id, object.un.spec1.gen_info.id_data.ID, object.un.spec1.spec_info1);
	printf("(spec2) object: info_id:%d obj_id:%d ID:%d spec2:'%s'\n", object.info_id, object.un.spec2.gen_info.obj_id, object.un.spec2.gen_info.id_data.ID, object.un.spec2.spec_info_array);

	return 0;
}
