#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent a task
struct Task
{
    int priority;
    char description[100];
};

// Structure to represent a priority queue
struct PriorityQueue
{
    struct Task *tasks;
    int capacity;
    int size;
};

// Function to initialize a priority queue with given capacity
struct PriorityQueue *createPriorityQueue(int capacity)
{
    struct PriorityQueue *pq = (struct PriorityQueue *)malloc(sizeof(struct PriorityQueue));
    pq->tasks = (struct Task *)malloc(capacity * sizeof(struct Task));
    pq->capacity = capacity;
    pq->size = 0;
    return pq;
}

// Helper function to swap two tasks
void swap(struct Task *a, struct Task *b)
{
    struct Task temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the min-heap property of the priority queue
void minHeapify(struct PriorityQueue *pq, int idx)
{
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;

    if (left < pq->size && pq->tasks[left].priority < pq->tasks[smallest].priority)
    {
        smallest = left;
    }

    if (right < pq->size && pq->tasks[right].priority < pq->tasks[smallest].priority)
    {
        smallest = right;
    }

    if (smallest != idx)
    {
        swap(&pq->tasks[idx], &pq->tasks[smallest]);
        minHeapify(pq, smallest);
    }
}

// Function to extract the task with the highest priority from the priority queue
struct Task extractMin(struct PriorityQueue *pq)
{
    if (pq->size <= 0)
    {
        struct Task nullTask;
        nullTask.priority = -1;
        return nullTask;
    }

    if (pq->size == 1)
    {
        pq->size--;
        return pq->tasks[0];
    }

    struct Task root = pq->tasks[0];
    pq->tasks[0] = pq->tasks[pq->size - 1];
    pq->size--;
    minHeapify(pq, 0);

    return root;
}

// Function to insert a new task into the priority queue
void insertTask(struct PriorityQueue *pq, int priority, const char *description)
{
    if (pq->size >= pq->capacity)
    {
        printf("Priority queue is full. Cannot insert more tasks.\n");
        return;
    }

    struct Task newTask;
    newTask.priority = priority;
    snprintf(newTask.description, sizeof(newTask.description), "%s", description);

    int i = pq->size;
    pq->tasks[i] = newTask;
    pq->size++;

    while (i != 0 && pq->tasks[(i - 1) / 2].priority > pq->tasks[i].priority)
    {
        swap(&pq->tasks[i], &pq->tasks[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int main()
{
    clock_t start, end;
    double cpu_time_used;

    int capacity;
    printf("Enter the capacity of the priority queue: ");
    scanf("%d", &capacity);

    struct PriorityQueue *pq = createPriorityQueue(capacity);

    int numTasks;
    printf("Enter the number of tasks: ");
    scanf("%d", &numTasks);

    for (int i = 0; i < numTasks; ++i)
    {
        int priority;
        char description[100];

        printf("Enter priority for task %d: ", i + 1);
        scanf("%d", &priority);

        printf("Enter description for task %d: ", i + 1);
        scanf("%s", description);

        insertTask(pq, priority, description);
    }

    start = clock(); // Record the start time

    // Extract tasks in priority order and print them
    printf("Tasks in priority order:\n");
    while (pq->size > 0)
    {
        struct Task task = extractMin(pq);
        if (task.priority != -1)
        {
            printf("Priority: %d, Description: %s\n", task.priority, task.description);
        }
    }

    end = clock(); // Record the end time
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time taken by the program: %f seconds\n", cpu_time_used);

    // Free the memory allocated for the priority queue
    free(pq->tasks);
    free(pq);

    return 0;
}