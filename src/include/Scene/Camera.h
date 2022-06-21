#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Scene/GObject.h"
#include "Scene/Player.h"
#include "Rendering/Frustum.h"
#include <Rendering/WindowManager.h>
//#include "Scene/Courier.h"

namespace GameEngine {
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP, 
        DOWN
    };

    // Default camera values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 0.2f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;


    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera: public GObject
    {
    public:
        // camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        glm::vec3 acceleration;
        // euler Angles
        float Yaw;
        float Pitch;
        // camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        float min_z = 10.f;
        float max_z = 40.f;
        Ref<GObject> player;

        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_projViewMatrix;

        float scr_width;
        float scr_height;

        ViewFrustum m_frustum;

        // constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
            updateFrustum();
            RevertAcceleration();
        }
        // constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
            updateFrustum();
            RevertAcceleration();

        }

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix()
        {
            m_viewMatrix = glm::lookAt(Position, Position + Front, Up);
            return m_viewMatrix;
        }

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(Camera_Movement direction)
        {
            float velocity = MovementSpeed;

            if (direction == FORWARD)
                acceleration.x +=  velocity;
            if (direction == BACKWARD)
                acceleration.x -= velocity;
            if (direction == LEFT)
                acceleration.y -= velocity;
            if (direction == RIGHT)
                acceleration.y += velocity;
        }

        void RevertAcceleration()
        {
            acceleration.x = 0;
            acceleration.y = 0;
        }

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw += xoffset;
            Pitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
            updateFrustum();
        }

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset)
        {
            Zoom -= (float)yoffset;
            if (Zoom < 1.0f)
                Zoom = 1.0f;
            if (Zoom > 45.0f)
                Zoom = 45.0f;
        }

        void setPos(glm::vec3 pos)
        {
            Position = pos;
        }

        glm::vec3 getPos() {
            return Position;
        }

        float getPitch()
        {
            return Pitch;
        }

        float getYaw()
        {
            return Yaw;
        }

        void updateFrustum()
        {
            m_projViewMatrix = m_projectionMatrix * m_viewMatrix;
            m_frustum.Update(m_projViewMatrix);
        }

        virtual void Move()
        {
            Position = player->get_transform().m_position + glm::vec3(0, 15, 12);
            //updateFrustum();

            //float x = player->get_transform().m_position.x - courier.x;
            //float y = player->get_transform().m_position.y - courier.y;
            //float distance = sqrtf( x * x + y * y) * 2;
            //distance = clamp(distance, min_z, max_z);
            //Position -= Front * distance;

            //std::cout << Position.x << " " << Position.y << " " << Position.z << "\n";
            /*
            if (acceleration.x != 0 || acceleration.y != 0)
            {
            }
            Position += Front * acceleration.x;
            Position += Right * acceleration.y;
            */

            updateCameraVectors();

        }

    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors()
        {
            // calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
            // also re-calculate the Right and Up vector
            Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            Up = glm::normalize(glm::cross(Right, Front));
        }
    };
}