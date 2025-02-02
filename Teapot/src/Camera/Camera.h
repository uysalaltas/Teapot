#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

#include "Teapot/Input.h"
#include "Teapot/KeyMacros.h"


namespace Teapot
{
    class Camera
    {
    public:

        Camera(const glm::vec3& eye, const glm::vec3& lookat, const glm::vec3& upVector, unsigned int& width, unsigned int& height)
            : m_eye(eye)
            , m_lookAt(lookat)
            , m_upVector(upVector)
            , m_width(width)
            , m_height(height)
        {
            UpdateViewMatrix();
        }

        glm::mat4x4& GetViewMatrix() { return m_viewMatrix; }
        glm::mat4x4& GetProjMatrix() { return m_projMatrix; }
        glm::vec3 GetEye() const { return m_eye; }
        glm::vec3 GetUpVector() const { return m_upVector; }
        glm::vec3 GetLookAt() const { return m_lookAt; }
        glm::vec3 GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
        glm::vec3 GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }
        float GetFOV() const { return m_fov; }

        void ActivateArcballCamera()
        {
            if (Input::IsMouseButtonPressed(KeyMacros::TEA_MOUSE_RIGHT))
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

        void ActivatePanCamera()
        {
            if (Input::IsMouseButtonPressed(KeyMacros::TEA_MOUSE_MIDDLE))
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

        void ActivateFreeMovement()
        {
            if (Input::IsKeyPressed(KeyMacros::TEA_KEY_W))
            {
                auto cameraFront = glm::normalize(m_lookAt - m_eye);
                auto right = glm::normalize(glm::cross(m_upVector, cameraFront));
                auto result = glm::normalize(glm::cross(right, cameraFront)) * m_freeCameraSpeed; // Camera Up
                m_eye -= result;
                m_lookAt -= result;
            }
            if (Input::IsKeyPressed(KeyMacros::TEA_KEY_S))
            {
                auto cameraFront = glm::normalize(m_lookAt - m_eye);
                auto right = glm::normalize(glm::cross(m_upVector, cameraFront));
                auto result = glm::normalize(glm::cross(right, cameraFront)) * m_freeCameraSpeed; // Camera Up
                m_eye += result;
                m_lookAt += result;
            }
            if (Input::IsKeyPressed(KeyMacros::TEA_KEY_A))
            {
                auto cameraFront = glm::normalize(m_lookAt - m_eye);
                auto result = glm::normalize(glm::cross(cameraFront, m_upVector)) * m_freeCameraSpeed; // Right
                m_eye -= result;
                m_lookAt -= result;
            }
            if (Input::IsKeyPressed(KeyMacros::TEA_KEY_D))
            {
                auto cameraFront = glm::normalize(m_lookAt - m_eye);
                auto result = glm::normalize(glm::cross(cameraFront, m_upVector)) * m_freeCameraSpeed; // Right
                m_eye += result;
                m_lookAt += result;
            }

            UpdateViewMatrix();
        }

        void ActivateZoomCamera()
        {
            if (Input::IsKeyPressed(KeyMacros::TEA_KEY_Q))
            {
                ProcessMouseScroll(1.0f);
            }
            else if (Input::IsKeyPressed(KeyMacros::TEA_KEY_E))
            {
                ProcessMouseScroll(-1.0f);
            }
        }

        void SetFOV(float fov)
        {
            m_fov = fov;
        }

        void SetFreeCameraMovementSpeed(float cameraSpeed)
        {
            m_freeCameraSpeed = cameraSpeed;
        }

        void UpdateViewMatrix()
        {
            m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
        }

        void UpdateProjMatrix()
        {
            m_projMatrix = glm::perspective(glm::radians(m_fov), (float) m_width / m_height, 0.1f, 200.0f * 20);
        }

        void SetCameraView(const glm::vec3& eye, const glm::vec3& lookat, const glm::vec3& up)
        {
            m_eye = eye;
            m_lookAt = lookat;
            m_upVector = up;
            UpdateViewMatrix();
        }

        void SetLookAt(const glm::vec3& lookat)
        {
            m_lookAt = lookat;
            UpdateProjMatrix();
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

        float m_freeCameraSpeed = 0.1f;

        bool firstMouseClick;
        bool firstRightMouseClick;
        glm::vec2 lastMousePosRightClick = glm::vec2(0.0f, 0.0f);

    private:
        void ArcBallCamera(float deltaX, float deltaY)
        {
            glm::vec4 position(GetEye().x, GetEye().y, GetEye().z, 1);
            glm::vec4 pivot(GetLookAt().x, GetLookAt().y, GetLookAt().z, 1);
            float deltaAngleX = (2 * glm::pi<float>() / m_width);
            float deltaAngleY = (glm::pi<float>() / m_height);
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

        void ProcessMouseScroll(float yoffset)
        {
            m_fov -= yoffset;
            if (m_fov < 0.1f)
                m_fov = 0.1f;
            if (m_fov > 90.0f)
                m_fov = 90.0f;
        }

    };
}
