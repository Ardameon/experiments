#ifndef CADAPTER_H
#define CADAPTER_H

typedef struct IDuck
{
    void (*fly_func)(struct IDuck *iduck);
    void (*quack_func)(struct IDuck *iduck);
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
void duck_quack(struct IDuck *iduck);
void duck_fly(struct IDuck *iduck);

Plane *plane_create(void);
void plane_destroy(Plane *plane);
void plane_fly(void);
void plane_rumble(void);

PlaneAdapter *plane_adapter_create(Plane *plane);
void plane_adapter_destroy(PlaneAdapter *adapter);
void plane_adapter_fly(struct IDuck *iduck);
void plane_adapter_quack(struct IDuck *iduck);

#endif
