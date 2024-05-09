#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

#include "Teapot/Input.h"
#include "Teapot/KeyMacros.h"

# define M_PI           3.14159265358979323846  /* pi */


namespace Teapot
{
    class Camera
    {
    public:

        Camera(glm::vec3& eye, glm::vec3& lookat, glm::vec3& upVector, unsigned int& width, unsigned int& height)
            : m_eye(eye)
            , m_lookAt(lookat)
            , m_upVector(upVector)
            , m_width(width)
            , m_height(height)
        {
            UpdateViewMatrix();
        }

        glm::mat4x4 GetViewMatrix() const { return m_viewMatrix; }
        glm::mat4x4 GetProjMatrix() const { return m_projMatrix; }
        glm::vec3 GetEye() const { return m_eye; }
        glm::vec3 GetUpVector() const { return m_upVector; }
        glm::vec3 GetLookAt() const { return m_lookAt; }
        glm::vec3 GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
        glm::vec3 GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }
        float GetFOV() const { return m_fov; }

        void CalculateArcballCamera()
        {
            if (Input::IsMouseButtonPressed(TEA_MOUSE_RIGHT))
            {
                if (firstMouseClick)
                {
                    lastMousePosRightClick.x = Input::GetMouseX();
                    lastMousePosRightClick.y = Input::GetMouseY();
                    firstMouseClick = false;
                }

                ArcBallCamera((lastMousePosRightClick.x - Input::GetMouseX()), (lastMousePosRightClick.y - Input::GetMouseY()));
                lastMousePosRightClick.x = Input::GetMouseX();
                lastMousePosRightClick.y = Input::GetMouseY();
            }
            else
            {
                firstMouseClick = true;
            }
        }

        void CalculatePanCamera()
        {
            if (Input::IsMouseButtonPressed(TEA_MOUSE_MIDDLE))
            {
                if (firstRightMouseClick)
                {
                    lastMousePosRightClick.x = Input::GetMouseX();
                    lastMousePosRightClick.y = Input::GetMouseY();
                    firstRightMouseClick = false;
                }

                PanCamera((Input::GetMouseX() - lastMousePosRightClick.x), (lastMousePosRightClick.y - Input::GetMouseY()));
                lastMousePosRightClick.x = Input::GetMouseX();
                lastMousePosRightClick.y = Input::GetMouseY();
            }
            else
            {
                firstRightMouseClick = true;
            }
        }

        void ZoomCamera()
        {
            if (Input::IsKeyPressed(TEA_KEY_W))
            {
                ProcessMouseScroll(1.0f);
            }
            else if (Input::IsKeyPressed(TEA_KEY_S))
            {
                ProcessMouseScroll(-1.0f);
            }
        }

        void SetFOV(float fov)
        {
            m_fov = fov;
        }

        void UpdateViewMatrix()
        {
            m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
        }

        void UpdateProjMatrix()
        {
            m_projMatrix = glm::perspective(glm::radians(m_fov), (float) m_width / m_height, 0.1f, 200.0f * 20);
        }

        void SetCameraView(glm::vec3& eye, glm::vec3& lookat, glm::vec3& up)
        {
            m_eye = eye;
            m_lookAt = lookat;
            m_upVector = up;
            UpdateViewMatrix();
        }

        void ProcessMouseScroll(float yoffset)
        {
            m_fov -= (float)yoffset;
            if (m_fov < 0.1f)
                m_fov = 0.1f;
            if (m_fov > 90.0f)
                m_fov = 90.0f;
        }

    private:
        glm::mat4x4 m_viewMatrix{};
        glm::mat4x4 m_projMatrix{};
        glm::vec3 m_eye{};                // Camera position in 3D
        glm::vec3 m_lookAt{};             // Point that the camera is looking at
        glm::vec3 m_upVector{};           // Orientation of the camera
        unsigned int& m_width;
        unsigned int& m_height;
        float m_fov = 45;
        float pan_speed = .5f;
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;

        bool firstMouseClick;
        bool firstRightMouseClick;
        glm::vec2 lastMousePosRightClick = glm::vec2(0.0f, 0.0f);

    private:
        void ArcBallCamera(float deltaX, float deltaY)
        {
            glm::vec4 position(GetEye().x, GetEye().y, GetEye().z, 1);
            glm::vec4 pivot(GetLookAt().x, GetLookAt().y, GetLookAt().z, 1);
            float deltaAngleX = (2 * M_PI / m_width);
            float deltaAngleY = (M_PI / m_height);
            float xAngle = deltaX * deltaAngleX;
            float yAngle = deltaY * deltaAngleY;

            float cosAngle = glm::dot(GetViewDir(), m_upVector);
            if (cosAngle * glm::sign(yAngle) > 0.99f)
                yAngle = 0;

            glm::mat4x4 rotationMatrixX(1.0f);
            rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_upVector);
            position = (rotationMatrixX * (position - pivot)) + pivot;

            glm::mat4x4 rotationMatrixY(1.0f);
            rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());
            glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

            SetCameraView(finalPosition, GetLookAt(), m_upVector);
        }

        void PanCamera(float deltaX, float deltaY)
        {
            deltaX *= 0.05f;
            deltaY *= 0.05f;
            glm::vec3 cameraFront = glm::normalize(m_lookAt - m_eye);
            glm::vec3 right = glm::normalize(glm::cross(cameraFront, m_upVector));
            glm::vec3 up = glm::normalize(m_upVector);
            m_eye -= deltaX * right + deltaY * up;
            m_lookAt -= deltaX * right + deltaY * up;

            UpdateViewMatrix();
        }

    };
}
