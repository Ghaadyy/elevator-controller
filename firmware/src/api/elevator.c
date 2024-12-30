/**
 * @file elevator.c
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @author Antoine Karam (antoinekaram1414@gmail.com)
 * @brief The scheduler for the elevator.
 *
 * @details Contains a scheduler for picking the next target floor the elevator has to go to. It stores two queues: one
 * for those that want to go up, one for those that want to go down.
 *
 * **Elementary algorithm**
 * - When the elevator is **completely** idle, it is free to go in any direction.
 * - When the elevator is going in a direction, it must preserve this direction until the respective queues becomes
 * empty.
 *
 * **Reordering the queues**
 * When we reach the destination (the top of the queue) and dequeue it, we have to reorder both queues to ensure the
 * invariant of the `queue_t`'s first and second are preserved. That is, the first part contains the floors which are
 * (resp. below, above) the current floor for the (resp. down, up) queues and the second part contains the floors which
 * are (resp. above, below) the cabin floor for the (resp. down, up) queue.
 *
 * @version 0.2
 * @date 2024-12-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../main.h"
#include "elevator.h"
#include "../sched/scheduler.h"
#include "motor.h"
#include "floor.h"
#include "queue.h"

typedef struct queue
{
    /*!< Contains the floors which are (resp. below, above) the current floor for the (resp. down, up) queues */
    floor_queue_t *first;

    /*!< Contains the floors which are (resp. above, below) the cabin floor for the (resp. down, up) queue. */
    floor_queue_t *second;
} queue_t;

typedef void (*add_to_q_fn)(floor_t floor);

/*!< The rate at which the scheduler will stop dispatching floors when a dequeue event happens. */
#define STOP_RATE (10)

/* Local prototypes */

void elevator_scheduler(s_task_handle_t me, s_task_msg_t **msg, void *arg);
void reorder_q(queue_t *q, add_to_q_fn add_to_q);
void add_to_up_q(floor_t);
void add_to_down_q(floor_t);

/* Global variables */

queue_t down_q = {NULL, NULL}; /*!< Queue containing the going down requests. */
queue_t up_q = {NULL, NULL};   /*!< Queue containing the going up requests. */

bool init_elevator_scheduler(void)
{
    return s_task_create(true, S_TASK_HIGH_PRIORITY, 100, elevator_scheduler, NULL, NULL);
}

/**
 * @brief Schedule and determine the next floor for the elevator to go to
 *
 * @param me
 * @param msg
 * @param arg
 */
void elevator_scheduler(s_task_handle_t me, s_task_msg_t **msg, void *arg)
{
    static uint8_t timer = 0;
    static bool dequeued = false;

    floor_t curr_flr = get_current_floor();
    direction_t curr_dir = get_motor_direction();
    static direction_t prev_dir = NONE;

    /* All queues are empty, nothing to do */
    if (is_empty(up_q.first) && is_empty(down_q.first) && is_empty(up_q.second) && is_empty(down_q.second))
    {
        prev_dir = NONE;
        return;
    }

    /* Dequeue from up */
    if (!is_empty(up_q.first) && curr_flr == top(up_q.first) && curr_dir == NONE && prev_dir != DOWN)
    {
        up_q.first = dequeue(up_q.first);
        reorder_q(&down_q, add_to_down_q);
        reorder_q(&up_q, add_to_up_q);
        dequeued = true;
    }

    /* Dequeue from down */
    if (!is_empty(down_q.first) && curr_flr == top(down_q.first) && curr_dir == NONE && prev_dir != UP)
    {
        down_q.first = dequeue(down_q.first);
        reorder_q(&down_q, add_to_down_q);
        reorder_q(&up_q, add_to_up_q);
        dequeued = true;
    }

    /* Dequeue from up if we were moving down to pick someone going up */
    if (is_empty(down_q.first) && !is_empty(up_q.second) && curr_flr == top(up_q.second) && curr_dir == NONE &&
        prev_dir == DOWN)
    {
        up_q.second = dequeue(up_q.second);
        reorder_q(&down_q, add_to_down_q);
        reorder_q(&up_q, add_to_up_q);
        dequeued = true;
    }

    /* Dequeue from down if we were moving up to pick someone going down */
    if (is_empty(up_q.first) && !is_empty(down_q.second) && curr_flr == top(down_q.second) && curr_dir == NONE &&
        prev_dir == UP)
    {
        down_q.second = dequeue(down_q.second);
        reorder_q(&down_q, add_to_down_q);
        reorder_q(&up_q, add_to_up_q);
        dequeued = true;
    }

    /* Delay of 1 second whenever we stop at a floor */
    if (dequeued)
    {
        timer++;

        if (timer >= STOP_RATE)
        {
            timer = 0;
            dequeued = false;
        }
        return;
    }

    if (curr_dir == UP)
    {
        enum floor target = top(up_q.first);

        /* If going up but no one wants to go up, serve those going down above us */
        if (is_empty(up_q.first) && !is_empty(down_q.second))
        {
            target = top(down_q.second);
        }

        goto_floor(target);

        prev_dir = curr_dir;
    }
    else if (curr_dir == DOWN)
    {
        enum floor target = top(down_q.first);

        /* If going down but no one wants to go down, serve those going up below us */
        if (is_empty(down_q.first) && !is_empty(up_q.second))
        {
            target = top(up_q.second);
        }

        goto_floor(target);

        prev_dir = curr_dir;
    }
    else /* Elevator is idle */
    {
        /* Continue moving in the previous downward direction if there are pending requests. */
        if (prev_dir == DOWN && !is_empty(down_q.first))
        {
            goto_floor(top(down_q.first));
        }
        /* Continue moving in the previous upward direction if there are pending requests. */
        else if (prev_dir == UP && !is_empty(up_q.first))
        {
            goto_floor(top(up_q.first));
        }
        else if (prev_dir == DOWN && !is_empty(up_q.second))
        {
            goto_floor(top(up_q.second));
        }
        else if (prev_dir == UP && !is_empty(down_q.second))
        {
            goto_floor(top(down_q.second));
        }
        /* Restart the elevator from a NONE state */
        else
        {
            /* Choose from up or down queue, if there are any requests */
            if (!is_empty(up_q.first))
            {
                goto_floor(top(up_q.first));
            }
            else if (!is_empty(down_q.first))
            {
                goto_floor(top(down_q.first));
            }
            else if (!is_empty(up_q.second))
            {
                goto_floor(top(up_q.second));
            }
            else if (!is_empty(down_q.second))
            {
                goto_floor(top(down_q.second));
            }
            else
            {
                /* Elevator will stay idle, do nothing. */
            }
        }
    }
}

/**
 * @brief Adds a request coming from the cabin floor to the appropriate queue based on the destination floor's position
 * relative to the current floor
 *
 * @param dst_floor The destination floor.
 */
void add_cabin_request(floor_t dst_floor)
{
    floor_t cabin_flr = get_current_floor();
    direction_t req_dir = dst_floor < cabin_flr ? DOWN : UP; /* Determine the direction of the request */

    if (req_dir == UP) /* Request to go UP */
    {
        add_to_up_q(dst_floor);
    }
    else /* Request to go DOWN */
    {
        add_to_down_q(dst_floor);
    }
}

/**
 * @brief Add a request coming externally from a floor. This request can be to go up or down.
 *
 * @param dst_floor The destination floor.
 * @param dir The direction desired by the request.
 */
void add_floor_request(floor_t dst_floor, direction_t dir)
{
    if (dir == UP) /* Request to go UP */
    {
        add_to_up_q(dst_floor);
    }
    else /* Request to go DOWN */
    {
        add_to_down_q(dst_floor);
    }
}

/**
 * @brief Reorders a queue.
 *
 * @details After changing floors (i.e. the cabin is now at a new floor), we have to reorder the queue's first and
 * second part. These parts are relative to the cabin floor. Since the cabin floor has changed, we have to reassign each
 * floor to the new part it belongs to.
 *
 * @param q The queue to be reordered.
 * @param add_to_q The function that adds to the queue.
 */
void reorder_q(queue_t *q, add_to_q_fn add_to_q)
{
    queue_t old_q = *q;
    floor_queue_t *curr_flr = old_q.first;
    q->first = NULL;
    q->second = NULL;

    while (!is_empty(curr_flr))
    {
        add_to_q(curr_flr->floor);
        floor_queue_t *old = curr_flr;
        curr_flr = curr_flr->next;
        free(old);
    }

    curr_flr = old_q.second;

    while (!is_empty(curr_flr))
    {
        add_to_q(curr_flr->floor);
        floor_queue_t *old = curr_flr;
        curr_flr = curr_flr->next;
        free(old);
    }
}

/**
 * @brief Add a floor to the up queue.
 *
 * @param dst_floor The destination floor.
 */
void add_to_up_q(floor_t dst_floor)
{
    floor_t cabin_flr = get_current_floor();
    /* Select the appropriate part of the up queue */
    floor_queue_t **curr_q = (dst_floor < cabin_flr) ? &up_q.second : &up_q.first;
    floor_queue_t *curr_flr = *curr_q;
    uint8_t pos = 1;

    /* Insert the floor in its correct position, maintaining ascending order */
    while (!is_empty(curr_flr))
    {
        if (dst_floor < curr_flr->floor)
        {
            *curr_q = insert(*curr_q, pos, dst_floor);
            break;
        }

        pos++;
        curr_flr = curr_flr->next;
    }

    if (is_empty(curr_flr))
    {
        *curr_q = insert(*curr_q, pos, dst_floor);
    }
}

/**
 * @brief Add a floor to the down queue.
 *
 * @param dst_floor The destination floor.
 */
void add_to_down_q(floor_t dst_floor)
{
    floor_t cabin_flr = get_current_floor();
    /* Select the appropriate part of the down queue */
    floor_queue_t **curr_q = (dst_floor > cabin_flr) ? &down_q.second : &down_q.first;
    floor_queue_t *curr_flr = *curr_q;
    uint8_t pos = 1;

    /* Insert the floor in its correct position, maintaining descending order */
    while (!is_empty(curr_flr))
    {
        if (dst_floor > curr_flr->floor)
        {
            *curr_q = insert(*curr_q, pos, dst_floor);
            break;
        }

        pos++;
        curr_flr = curr_flr->next;
    }

    if (is_empty(curr_flr))
    {
        *curr_q = insert(*curr_q, pos, dst_floor);
    }
}