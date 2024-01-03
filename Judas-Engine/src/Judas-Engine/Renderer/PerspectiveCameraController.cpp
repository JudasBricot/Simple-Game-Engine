#include "jepch.h"
#include "PerspectiveCameraController.h"

#include "Core/Core.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace Judas_Engine
{
    PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float camNear, float camFar)
        : m_Fov(fov), m_AspectRatio(aspectRatio), m_Near(camNear), m_Far(camFar), m_Camera(fov, aspectRatio, camNear, camFar) { }

    void PerspectiveCameraController::OnUpdate(Timestep ts)
    {
        glm::vec3 translation = glm::vec3(0.0f);

        if (Input::IsKeyPressed(JE_KEY_R))
        {
            m_Camera.Reset();
        }

        if (Input::IsKeyPressed(JE_KEY_W))
            translation.y += m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(JE_KEY_S))
            translation.y -= m_CameraTranslationSpeed * ts;
        if (Input::IsKeyPressed(JE_KEY_A))
            translation.x += m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(JE_KEY_D))
            translation.x -= m_CameraTranslationSpeed * ts;

        if(translation.length() > 0.0f)
            m_Camera.TranslateLocal(translation);
    }

    void PerspectiveCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
        dispatcher.Dispatch<MouseDraggedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseDragged));
        dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
    }

    bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        glm::vec3 target = m_Camera.GetLocalTargetPosition();
        float zoomIntensity = glm::sqrt(target.x * target.x + target.y * target.y + target.z * target.z) / 2.0f;
        float zoom = zoomIntensity * e.GetYOffset();

        zoom = abs(zoom) < m_SlowingZoomZoneRadius ? zoom / m_SlowingFactor : zoom;

        if (m_Camera.GetLocalTargetPosition().z - zoom > m_MaxZoomLevel)
        {
            m_Camera.TranslateLocal({ 0.0f, 0.0f, zoom });
            m_Camera.TranslateTargetLocal({ 0.0f, 0.0f, -zoom });
        }

        return true;
    }

    bool PerspectiveCameraController::OnMouseDragged(MouseDraggedEvent& e)
    {
        glm::vec3 axis = glm::normalize(glm::vec3(e.GetYOffset(), -e.GetXOffset(), 0.0f));
        float draggingLength = glm::sqrt(e.GetXOffset() * e.GetXOffset() + e.GetYOffset() * e.GetYOffset());

        m_Camera.RotateAroundAxis(axis, m_DraggingSpeed * draggingLength);

        return true;
    }

    bool PerspectiveCameraController::OnWindowResized(WindowResizedEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(m_Fov, m_AspectRatio, m_Near, m_Far);

        return true;
    }
}
