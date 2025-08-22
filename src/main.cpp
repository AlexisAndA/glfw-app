#include <glad/glad.h>

#include "graphics/shader.h"
#include "graphics/camera.h"
#include "glfw_utils.h"
#include "graphics/window.h"
#include "model.h"
#include "light.h"
#include "world.h"

constexpr uint16_t SCREEN_WIDTH = 1280;
constexpr uint16_t SCREEN_HEIGHT = 720;

// timing

int main()
{
    init_glfw();

    // glfw window creation
    // --------------------
    Window window = Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Glfw");

    init_glad();

    World world; // Everything is abstracted!

    Object *floor = new Object("assets/floor.obj", glm::vec3(1.0f), Graphics::Texture("assets/grass.png"), Graphics::Texture("assets/grass.png"));
    floor->AddChild(new Object("assets/monkey.obj", glm::vec3(5.0f, 2.5f, 2.0f), Graphics::Texture("assets/diamond.png"), Graphics::Texture("assets/diamond.png")));
    floor->AddChild(new Object("assets/bunny.obj", glm::vec3(2.0f, 1.0f, 1.0f), Graphics::Texture("assets/wool.png"), Graphics::Texture("assets/wool.png")));
    floor->AddChild(new Object("assets/teapot.obj", glm::vec3(-1.0f, 1.0f, 2.0f)));
    floor->AddChild(new Object("assets/bunny.obj", glm::vec3(-3.8f, 1.0f, -2.3f)));

    world.AddObject(floor);

    world.AddLight(new Light(glm::vec3(-2.0f, 4.0f, -1.0f), Graphics::Color(255, 0, 0, 255)));
    world.AddLight(new Light(glm::vec3(2.3f, 5.3f, -4.0f), Graphics::Color(0, 255, 0, 255)));
    world.AddLight(new Light(glm::vec3(-4.0f, 5.0f, -5.0f), Graphics::Color(0, 0, 255, 255)));
    world.AddLight(new Light(glm::vec3(0.0f, 5.0f, -3.0f), Graphics::Color(255, 255, 255, 255)));

    Camera camera = Camera(glm::vec3(0.0f, 5.0f, 10.0f));

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float time = 0.0f;

    while (!window.ShouldClose())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        time += deltaTime;

        window.ProcessInput();
        camera.ProcessInput(window, deltaTime);

        // render
        // ------
        window.ClearBackground(Graphics::Color(77, 150, 227, 255));

        world.RenderDepthMaps(window);
        // world.objects[0]->RenderDepth();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.Render(window, camera);
        // world.lights[0]->RenderDebugQuad();

        window.Update();
    }

    world.CleanUp();

    glfwTerminate();
    return 0;
}