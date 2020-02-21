#include "render_engine.h"

RenderEngine* RenderEngine::singleton = nullptr;

RenderEngine::RenderEngine() {
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

    glClearColor(0.5294f, 0.8078f, 0.9216f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
}

RenderEngine::~RenderEngine() {}

RenderEngine* RenderEngine::get_instance() {
	return RenderEngine::singleton;
}

void RenderEngine::init() {
    singleton = new RenderEngine();
	TextureManager::init();
}

void RenderEngine::destroy() {
	TextureManager::destroy();
    delete singleton;
}

void RenderEngine::render(GLfloat delta_time, GLfloat update_lag) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.use_program(update_lag);

    for (std::vector<RenderEntity*>::iterator it = render_entity_list.begin();
        it != render_entity_list.end(); it++) {
        if (*it != nullptr) {
            (*it)->render(camera);
        }
    }
}

void RenderEngine::add_entity(RenderEntity* entity) {
    // add the entity to the first empty spot
    bool inserted = false;
    for (int i = 0; i < render_entity_list.size(); i++) {
        if (render_entity_list[i] == nullptr) {
            render_entity_list[i] = entity;
            inserted = true;
            break;
        }
    }

    if (!inserted) {
        render_entity_list.push_back(entity);
    }
}

void RenderEngine::remove_entity(RenderEntity* entity) {
    for (int i = 0; i < render_entity_list.size(); i++) {
        if (render_entity_list[i] == entity) {
            render_entity_list[i] = nullptr;
            break;
        }
    }
}

WavefrontOBJEntity* RenderEngine::load_wv_obj(std::string file_path) {
    WavefrontOBJEntity* entity = wf_obj_manager.create_instance(file_path);
    add_entity(entity);
    return entity;
}

void RenderEngine::remove_wv_obj(WavefrontOBJEntity* entity) {
    remove_entity(entity);
    wf_obj_manager.remove_instance(entity);
}


Camera& RenderEngine::get_camera() {
    return camera;
}
