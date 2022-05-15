#include "GUI/GUIRenderer.h"

static float verts[] = {
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     
     1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,

};

GameEngine::GUIRenderer::GUIRenderer(Ref<Camera> camera) : m_vbo(GL_ARRAY_BUFFER), m_camera(camera)
{
    m_guiShader = CreateRef<Shader>("res/shaders/GUI.vert", "res/shaders/GUI.frag");
    m_guiShader->use();
    m_guiShader->setInt("ourTexture", 0);
    glm::mat4 ortho = glm::ortho(0.0f, (float)GuiComponent::m_scr_width, 0.0f, (float)GuiComponent::m_src_height);
    m_guiShader->setMat4("projectionMatrix", ortho);
    std::cout << "GUI Renderer created\n";
    m_vao.Bind();
    m_vbo.BufferData(sizeof(verts), verts, GL_STATIC_DRAW);
    m_vbo.VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    m_vao.Unbind();
}

GameEngine::GUIRenderer::~GUIRenderer()
{
}

void GameEngine::GUIRenderer::Render(std::vector<Ref<GuiComponent>> toDraw)
{
    m_vao.Bind();
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    m_guiShader->use();
    glActiveTexture(GL_TEXTURE0);
    //std::cout << "Render";

    for (auto c : toDraw)
    {
        c->getTexture()->Bind();
        m_guiShader->setMat4("transfomationMatrix", c->getTransformatiomMatrix());
        m_guiShader->setVec3("aColor", c->getColor());
        m_guiShader->setFloat("alpha", c->getOpacity());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    m_vao.Unbind();
    glEnable(GL_DEPTH_TEST);


}
