/*=========================================================*/
/* Observer interface */
typedef struct observert_t
{
    void (*subject_updated_func) (struct observert_t *obsrv);
    void (*display_func)         (struct observert_t *obsrv);
    void *subject;
    char name[10];
} observert_t;

/*=========================================================*/
/* Subject interface */
typedef struct subject_t
{
    void (*update_state_func) (struct subject_t *subj);
    void (*add_observer_func) (struct subject_t *subj, observert_t *obsrv);
    void (*rem_observer_func) (struct subject_t *subj, observert_t *obsrv);
    int  (*get_temp_func)     (struct subject_t *subj);
    int  (*get_humad_func)    (struct subject_t *subj);

    observert_t **observer;
    int observers_count;
} subject_t;

/*=========================================================*/

typedef struct meteo_station_t
{
    subject_t subj;

    int temperature;
    int humadity;
} meteo_station_t;

/*=========================================================*/

typedef struct temperature_display_t
{
    observert_t obsrv;
    int temp;
} temperature_display_t;

typedef struct humadity_display_t
{
    observert_t obsrv;
    int humad;
} humadity_display_t;

typedef struct combo_display_t
{
    observert_t obsrv;
    int temp;
    int humad;
} combo_display_t;