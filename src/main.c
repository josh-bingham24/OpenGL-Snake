#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>
#include <stb_image.h>
#include <learnopengl/camera.h>

// me
#include <me/shape_factory.h>
#include <me/snake.h>

// glm
#include <cglm/cglm.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, float deltaTime);


// ----- Shader -----
Shader shader;

// ----- Camera -----
Camera camera;
float lastX = 400, lastY = 300;
bool firstMouse = true;

// ----- Time -----
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// ----- Shapes -----
Shape *cheeseball;

// ----- SNAKE -----
Snake snake;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // initialize window
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }    

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback); 


    // setup shaders
    Shader_init(&shader, "../src/shader_v.txt", "../src/shader_f.txt");

    // setup camera
    camera = Camera_init(STATIC, 60.0f, 0.1f);

    // create snake
    snake = CreateSnake(12, 1.0f);
    cheeseball = CreateCircle(0.3f, 36, (vec3){ 0.5f, 2.5f, 0.0f });
    // MoveCheeseball(&snake, cheeseball);


    // vertex handling
    unsigned int VBO, VBO2, VAO, VAO2;
    // glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO2);

    // Snake
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 5 * 6 * sizeof(float), snake.links[0]->vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Cheeseball
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, 3 * 5 * 36 * sizeof(float), cheeseball->vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // EBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    //              GL_STATIC_DRAW);

    // unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // enable z-buffer
    glEnable(GL_DEPTH_TEST); 


    // setup textures
    unsigned int texture;
    unsigned char *data;
    int width, height, nrChannels;

    // texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../resources/awesomeface.png", &width, &height, &nrChannels,
                     0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      printf("Failed to load face texture");
    }
    stbi_image_free(data);


    // enable mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double lastTime = glfwGetTime();

    // main loop
    while(!glfwWindowShouldClose(window))
    {   
        // update delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        // process user input
        processInput(window, deltaTime);

        // clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // shader config
        useShader(&shader);
        setInt(&shader, "texture1", 0);
        // setInt(&shader, "texture", 1);

        // bind textures
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);


        // transforms
        mat4 model, view, projection;

        // create view
        glm_look(camera.position, camera.front, camera.up, view);            // draw
        // glm_perspective(glm_rad(camera.fov), 4.0f / 3.0f, 0.1f, 100.0f, projection);
        glm_ortho(-8.0f, 8.0f, -6.0f, 6.0f, 0.1f, 100.0f, projection);
        
        setMat4(&shader, "view", view);
        setMat4(&shader, "projection", projection);

        // Move the snake
        if (currentFrame - lastTime >= 0.2) {
            if (!MoveSnake(&snake, cheeseball)) {
                printf("You crashed!\n");
                break;
            }

            lastTime = currentFrame;
        }

        // draw cheeseball
        glBindVertexArray(VAO2);
        glm_mat4_identity(model);
        glm_translate(model, cheeseball->position);
        setMat4(&shader, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 108);

        // draw all snake links
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < snake.length; i++) {
            glm_mat4_identity(model);
            glm_translate(model, snake.links[i]->position);
            setMat4(&shader, "model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    if(snake.length >= 140) {
        printf("Winner!\n");
    }

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    HandleScroll(&camera, yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    HandleMouse(&camera, xoffset, yoffset);
}

void processInput(GLFWwindow *window, float deltaTime)
{
    // close window on escape
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // player movement
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { 
        if (snake.currentMovement != DOWN)
            snake.currentMovement = UP;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (snake.currentMovement != UP)
            snake.currentMovement = DOWN;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (snake.currentMovement != RIGHT)
            snake.currentMovement = LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (snake.currentMovement != LEFT)
            snake.currentMovement = RIGHT;
    }
}
