#include "jepch.h"
#include "PerspectiveCameraController.h"

#include "Core/Core.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace Judas_Engine
{
    PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float camNear, float camFar, bool rotation = false)
        : m_Fov(fov), m_AspectRatio(aspectRatio), m_Near(camNear), m_Far(camFar), m_Camera(fov, aspectRatio, camNear, camFar), m_Rotation(rotation)
    {

    }

    void PerspectiveCameraController::OnUpdate(Timestep ts)
    {
        glm::vec3 translation = glm::vec3(0.0f);

        if (Input::IsKeyPressed(JE_KEY_W))
            translation.y += m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(JE_KEY_S))
            translation.y -= m_CameraTranslationSpeed * ts;
        if (Input::IsKeyPressed(JE_KEY_A))
            translation.x += m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(JE_KEY_D))
            translation.x -= m_CameraTranslationSpeed * ts;

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(JE_KEY_Q))
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            else if (Input::IsKeyPressed(JE_KEY_E))
                m_CameraRotation += m_CameraRotationSpeed * ts;

            m_Camera.SetRotation(m_CameraRotation);
        }

        if(translation.length() > 0.0f)
            m_Camera.TranslateLocal(translation);
    }

    void PerspectiveCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
    }

    bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_Camera.Translate({0.0f, 0.0f, e.GetYOffset()});

        return true;
    }

    bool PerspectiveCameraController::OnWindowResized(WindowResizedEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(m_Fov, m_AspectRatio, m_Near, m_Far);

        return true;
    }
}
