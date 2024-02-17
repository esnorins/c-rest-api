#include <stdio.h>
#include <stdbool.h>
#include <ulfius.h>
#include <jansson.h>

#define HTTP_OK 200
#define ID_SIZE 8
#define PORT 8080

#define REST_TASKS "/api/tasks"

typedef struct Todo {
    int id;       // ID_SIZE digits max; id;
    bool done;    // is done;
    char *desc;   // description;
} Todo;

Todo todos[] = {
    {
        1,
        false,
        "Check out Haskell, this time for real.",
    },
    { 
        2,
        false,
        "Learn anonymous functions in Rust.",
    },
    {
        3,
        true,
        "Read 5 pages of a book.",
    },
    {
        4,
        true,
        "Take a walk in the park. Pun intended.",
    },
};
#define TODOS_C sizeof(todos)/sizeof(todos[0])

json_t *todo_to_json_t(const Todo todo) {
    json_t *id_json = json_integer(todo.id);
    json_t *done_json = json_boolean(todo.done);
    json_t *desc_json = json_string(todo.desc);

    json_t *todo_object = json_object();
    json_object_set(todo_object, "id", id_json);
    json_object_set(todo_object, "done", done_json);
    json_object_set(todo_object, "desc", desc_json);

    return todo_object;
}

int callback_tasks(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *array = json_array();

    for (size_t i = 0; i < TODOS_C; i++) {
        json_t *todo_json = todo_to_json_t(*((struct Todo *)user_data+i));
        json_array_append_new(array, todo_json);
    }

    ulfius_set_json_body_response(response, HTTP_OK, array);

    return U_CALLBACK_CONTINUE;
}

int main(void) {
    struct _u_instance instance;

    if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
        fprintf(stderr, "[ERR]: Ulfius framework init failed.\n");
        return -1;
    }

    ulfius_add_endpoint_by_val(&instance, "GET", REST_TASKS, NULL, 0, &callback_tasks, (void *)todos);

    if (ulfius_start_framework(&instance) != U_OK) {
        fprintf(stderr, "[ERR]: Start Ulfius framework failed.\n");
        return -1;
    } else {
        printf("[INFO]: Ulfius framework started on port: %d\n", instance.port);
        getchar();
    }
    printf("[INFO]: Framework ended.\n");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return 0;
}
