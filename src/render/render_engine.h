#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H


#include "camera.h"
#include "render_entity.h"
#include "render_gui.h"
#include "wf_obj_manager.h"
#include "texture_manager.h"
#include "font_manager.h"
#include <glad/glad.h>
#include <vector>
#include <list>

class RenderEngine {
private:
    static RenderEngine* singleton;
    GLfloat window_width;
    GLfloat window_height;
    RenderEngine();
    ~RenderEngine();
    
    WavefrontOBJManager wf_obj_manager;
    FontManager font_manager;
    std::list<RenderGui*> render_gui_list;
    std::vector<RenderEntity*> render_entity_list;
    GLuint vertex_array_id;
    Camera camera;
    
public:
    // singleton methods
	static RenderEngine* get_instance();
    // must be called before any call to get_instance()
    static void init();
    static void destroy();

    // set the dimensions of the window in pixels
    void set_window_dimensions(unsigned int width, unsigned int height);
    unsigned int get_window_width();
    unsigned int get_window_height();

    // renders all render entities
    // delta_time specifies the time since the last render call
    // update_lag specifies the ratio of the time since the physics update
    // to the length of the physics time step
    // update_lag must be in the range [0, 1]
    // this allows for interpolation of entity orientation
    void render(GLfloat delta_time, GLfloat update_lag);

    // an added render entity will be drawn every render call
    void add_entity(RenderEntity* entity);

    // remove an entity from the rendering process
    // this function does not deallocate the entity
    void remove_entity(RenderEntity* entity);

    // an added render entity will be drawn every render call
    void add_gui(RenderGui* gui);

    // remove an entity from the rendering process
    // this function does not deallocate the gui
    void remove_gui(RenderGui* gui);

    // loads a Wavefront OBJ instance
    WavefrontOBJEntity* load_wv_obj(std::string file_path);
    // removes a Wavefront OBJ instance
    void remove_wv_obj(WavefrontOBJEntity* entity);

    Camera& get_camera();

    // gets the font manager
    FontManager& get_font_manager();
};

#endif
