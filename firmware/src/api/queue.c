/**
 * @file queue.c
 * @author Ghady Youssef (ghadyyi@gmail.com)
 * @author Antoine Karam (antoinekaram1414@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-12-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../main.h"
#include "queue.h"

/*Local prototypes*/
floor_queue_t *create_queue(floor_t flr);

/**
 * @brief Create a queue node.
 *
 * @param flr
 * @return floor_queue_t*
 */
floor_queue_t *create_queue(floor_t flr)
{
    floor_queue_t *new_node = malloc(sizeof(floor_queue_t));

    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->floor = flr;
    return new_node;
}

floor_t top(floor_queue_t *q)
{
    return q->floor;
}

floor_queue_t *insert(floor_queue_t *head, uint8_t pos, floor_t new_floor)
{
    floor_queue_t *new_node = create_queue(new_floor);

    // Insertion at the beginning
    if (pos == 1)
    {
        new_node->next = head;

        // If the linked list is not empty, set the prev of head to new node
        if (head != NULL)
        {
            head->prev = new_node;
        }

        // Set the new node as the head of linked list
        head = new_node;
        return head;
    }

    floor_queue_t *curr = head;

    // Traverse the list to find the node before the insertion point
    for (uint8_t i = 1; i < pos - 1 && curr != NULL; ++i)
    {
        curr = curr->next;
    }

    // If the position is out of bounds
    if (curr == NULL)
    {
        free(new_node);
        return head;
    }

    // Set the prev of new node to curr
    new_node->prev = curr;

    // Set the next of new node to next of curr
    new_node->next = curr->next;

    // Update the next of current node to new node
    curr->next = new_node;

    // If the new node is not the last node, update the prev of next node to new node
    if (new_node->next != NULL)
    {
        new_node->next->prev = new_node;
    }

    // Return the head
    return head;
}

floor_queue_t *dequeue(floor_queue_t *q)
{
    if (is_empty(q))
        return NULL;

    floor_queue_t *temp = q;

    q = q->next;

    if (is_empty(q))
        q->prev = NULL;

    free(temp);
    return q;
}

bool is_empty(floor_queue_t *q)
{
    return q == NULL;
}