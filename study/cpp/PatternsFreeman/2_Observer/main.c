#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "observer.h"

#define MAX_OBSERVERS 10

/*=========================================================*/

void temp_disp_init(temperature_display_t *disp, subject_t *station);
void temp_disp_update(observert_t *obsrv);
void temp_disp_show(observert_t *obsrv);
void humad_disp_init(humadity_display_t *disp, subject_t *station);
void humad_disp_update(observert_t *obsrv);
void humad_disp_show(observert_t *obsrv);
void combo_disp_init(combo_display_t *disp, subject_t *station);
void combo_disp_update(observert_t *obsrv);
void combo_disp_show(observert_t *obsrv);

void station_init(meteo_station_t *station);
void station_observer_add(subject_t *station_subj, observert_t *obsrv);
void station_observer_rem(subject_t *station_subj, observert_t *obsrv);
int  station_temp_get(subject_t *station_subj);
int  station_humad_get(subject_t *station_subj);
void station_state_update(subject_t *station_subj);


/*=========================================================*/

void temp_disp_init(temperature_display_t *disp, subject_t *station)
{
    disp->obsrv.subject_updated_func = &temp_disp_update;
    disp->obsrv.display_func         = &temp_disp_show;
    disp->obsrv.subject              = station;
    strcpy(disp->obsrv.name, "TEMP_DISPLAY");

    station->add_observer_func(station, (observert_t *)disp);
}

void temp_disp_update(struct observert_t *obsrv)
{
    temperature_display_t *disp = (temperature_display_t *)obsrv;

    disp->temp = ((subject_t *)obsrv->subject)->get_temp_func(obsrv->subject);

    obsrv->display_func(obsrv);
}

void temp_disp_show(struct observert_t *obsrv)
{
    temperature_display_t *disp = (temperature_display_t *)obsrv;

    printf("%s:  temp:  %-3d\n", obsrv->name, disp->temp);
}

void humad_disp_init(humadity_display_t *disp, subject_t *station)
{
    disp->obsrv.subject_updated_func = &humad_disp_update;
    disp->obsrv.display_func         = &humad_disp_show;
    disp->obsrv.subject              = station;
    strcpy(disp->obsrv.name, "HUMAD_DISPLAY");

    station->add_observer_func(station, (observert_t *)disp);
}

void humad_disp_update(struct observert_t *obsrv)
{
    humadity_display_t *disp = (humadity_display_t *)obsrv;

    disp->humad = ((subject_t *)obsrv->subject)->get_humad_func(obsrv->subject);

    obsrv->display_func(obsrv);
}

void humad_disp_show(struct observert_t *obsrv)
{
    humadity_display_t *disp = (humadity_display_t *)obsrv;

    printf("%s: humad: %-3d\n", obsrv->name, disp->humad);
}

void combo_disp_init(combo_display_t *disp, subject_t *station)
{
    disp->obsrv.subject_updated_func = &combo_disp_update;
    disp->obsrv.display_func         = &combo_disp_show;
    disp->obsrv.subject              = station;
    strcpy(disp->obsrv.name, "COMBO_DISPLAY");

    station->add_observer_func(station, (observert_t *)disp);
}

void combo_disp_update(struct observert_t *obsrv)
{
    combo_display_t *disp = (combo_display_t *)obsrv;

    disp->humad = ((subject_t *)obsrv->subject)->get_humad_func(obsrv->subject);
    disp->temp = ((subject_t *)obsrv->subject)->get_temp_func(obsrv->subject);

    obsrv->display_func(obsrv);
}

void combo_disp_show(struct observert_t *obsrv)
{
    combo_display_t *disp = (combo_display_t *)obsrv;

    printf("%s: temp:  %-3d humad: %-3d\n", obsrv->name, disp->temp, disp->humad);
}

/*=========================================================*/

void station_init(meteo_station_t *station)
{
    subject_t *station_subj = (subject_t *)station;

    station_subj->update_state_func = &station_state_update;
    station_subj->add_observer_func = &station_observer_add;
    station_subj->rem_observer_func = &station_observer_rem;
    station_subj->get_temp_func     = &station_temp_get;
    station_subj->get_humad_func    = &station_humad_get;

    station_subj->observer = calloc(MAX_OBSERVERS, sizeof(*station_subj->observer));
    station_subj->observers_count = 0;

    station->temperature = 0;
    station->humadity = 0;
}

void station_observer_add(subject_t *station_subj, observert_t *obsrv)
{
    station_subj->observer[station_subj->observers_count++] = obsrv;
}

void station_observer_rem(subject_t *station_subj, observert_t *obsrv)
{
    int i;

    for (i = 0; i < station_subj->observers_count; i++)
    {
        if (station_subj->observer[i] == obsrv)
        {
            station_subj->observers_count--;
            break;
        }
    }

    for (; i < station_subj->observers_count; i++)
    {
        station_subj->observer[i] = station_subj->observer[i + 1];
    }
}

int station_temp_get(subject_t *station_subj)
{
    meteo_station_t *station = (meteo_station_t *)station_subj;

    return station->temperature;
}

int station_humad_get(subject_t *station_subj)
{
    meteo_station_t *station = (meteo_station_t *)station_subj;

    return station->humadity;
}

void station_state_update(subject_t *station_subj)
{
    int i;

    printf("==== Weather conditions were changed ======================\n");

    for (i = 0; i < station_subj->observers_count; i++)
    {
        station_subj->observer[i]->subject_updated_func(station_subj->observer[i]);
    }
}

/*=========================================================*/

int main(int argc, char const *argv[])
{
    meteo_station_t station;

    temperature_display_t temp_display;
    humadity_display_t    humad_display;
    combo_display_t       combo_display;

    station_init(&station);

    temp_disp_init(&temp_display, (subject_t *)&station);
    humad_disp_init(&humad_display, (subject_t *)&station);
    combo_disp_init(&combo_display, (subject_t *)&station);

    srand(time(0));

    while (1)
    {
        station.temperature = rand() % 70 - 30;
        station.humadity    = rand() % 89 + 10;
        station.subj.update_state_func(&station.subj);

        fflush(stdout);
        sleep(1);

        station.subj.rem_observer_func(&station.subj, (observert_t *) &combo_display);
    }

    (void)argc;
    (void)argv;
    return 0;
}

