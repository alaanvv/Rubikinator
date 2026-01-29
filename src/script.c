#include "canvas.h"

u32 shader;

CanvasConfig config = { 
  .title = "MOUD", 
  .fullscreen = 0, 
  .screen_size = 0.5, 
  .clear_color = PURPLE
};

Camera cam = {
  .pos = { 0, 0, 5 },
  .fov = PI4,
  .near_plane = 0.01,
  .far_plane = 100,
};

// ---


typedef struct {
  u8 yaw, pitch, roll;
} Direction;

Direction rcube[3][3][3] = {0};

// ---

int main() {
  canvas_init(&cam, config);

  Material m_cube   = { WHITE, 1, 0.6, .tex = GL_TEXTURE0 };

  // Light
  PntLig light = { WHITE, { 0 }, 1, 0.07, 0.017 };

  // Model
  Model* cube   = model_create("cube",   &m_cube,   1);

  // Texture
  canvas_create_texture(GL_TEXTURE0, "rubiks", TEXTURE_DEFAULT);

  // Shader
  shader = shader_create_program("obj");
  generate_proj_mat(&cam, shader);
  generate_view_mat(&cam, shader);
  canvas_set_pnt_lig(shader, light, 0);

  while (!glfwWindowShouldClose(cam.window)) {
    // 3D Drawing
    glUseProgram(shader);

    for (u8 x = 0; x < 3; x++)
      for (u8 y = 0; y < 3; y++)
        for (u8 z = 0; z < 3; z++) {
          model_bind(cube, shader);

          glm_rotate(cube->model, PI2 * rcube[x][y][z].yaw,   VEC3(0, 1, 0));
          glm_rotate(cube->model, PI2 * rcube[x][y][z].pitch, VEC3(0, 0, 1));
          glm_rotate(cube->model, PI2 * rcube[x][y][z].yaw,   VEC3(1, 0, 0));

          glm_translate(cube->model, VEC3(-1 + x, -1 + y, -1 + z));

          model_draw(cube, shader);
        }

    // Finish
    glfwSwapBuffers(cam.window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwPollEvents();
    camera_handle_window_events(&cam);

    update_fps(&cam);
  }

  glfwTerminate();
  return 0;
}
