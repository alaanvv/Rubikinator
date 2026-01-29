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

    model_bind(cube, shader);
    model_draw(cube, shader);

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
