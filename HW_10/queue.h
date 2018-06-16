#ifndef QUEUE_H_
#define QUEUE_H_


/** Elemento della coda.
 *
 */
typedef struct Node {
    void        * data;
    struct Node * next;
} Node_t;

/** Struttura dati coda.
 *
 */
typedef struct Queue {
    Node_t        *head;
    Node_t        *tail;
    unsigned long  qlen;
} Queue_t;


/** Alloca ed inizializza una coda. Deve essere chiamata da un solo 
 *  thread (tipicamente il thread main).
 *   \param nrow numero righe
 *   \param numero colonne
 *
 *   \retval NULL se si sono verificati problemi nell'allocazione (errno settato)
 *   \retval q puntatore alla coda allocata
 */
Queue_t *initQueue();

/** Cancella una coda allocata con initQueue. Deve essere chiamata da
 *  da un solo thread (tipicamente il thread main).
 *  
 *   \param q puntatore alla coda da cancellare
 */
void deleteQueue(Queue_t *q);

/** Inserisce un dato nella coda.
 *   \param data puntatore al dato da inserire
 *  
 *   \retval 0 se successo
 *   \retval -1 se errore (errno settato opportunamente)
 */
int    push(Queue_t *q, void *data);

/** Estrae un dato dalla coda.
 *
 *  \retval data puntatore al dato estratto.
 */
void  *pop(Queue_t *q);

/** Ritorna la lunghezza della coda. L'accesso non è in mutua esclusione !
 *
 *  \retval lunghezza della coda.
 */
unsigned long length(Queue_t *q);

/** Stampa sullo standard output la lunghezza della coda e gli elementi in essa contenuti
 *
 *   \param q puntatore alla coda 
 *   \param f funzione che trasforma data in una stringa. La stringa e' contenuta in buf che 
 *   ha una size di s bytes.
 */
void queueStatus(Queue_t *q, void (*f)(char *buf, size_t s, void *data));


#endif /* QUEUE_H_ */
