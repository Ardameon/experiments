#ifndef CADAPTER_H
#define CADAPTER_H

struct IDuck;

typedef void (*iduck_fly_func)(struct IDuck *iduck);
typedef void (*iduck_quack_func)(struct IDuck *iduck);

typedef struct IDuck
{
    iduck_fly_func   fly_func;
    iduck_quack_func quack_func;
} IDuck;

typedef struct
{
    IDuck base;
} Duck;

typedef struct
{
    void (*fly_func)(void);
    void (*rumble_func)(void);
} Plane;

typedef struct
{
    IDuck base;
    Plane *plane;
} PlaneAdapter;

void quack(IDuck *duck);
void fly(IDuck *duck);

Duck *duck_create(void);
void duck_destroy(Duck *duck);
void duck_quack(IDuck *iduck);
void duck_fly(IDuck *iduck);

Plane *plane_create(void);
void plane_destroy(Plane *plane);
void plane_fly(void);
void plane_rumble(void);

PlaneAdapter *plane_adapter_create(Plane *plane);
void plane_adapter_destroy(PlaneAdapter *adapter);
void plane_adapter_fly(IDuck *iduck);
void plane_adapter_quack(IDuck *iduck);

#endif
