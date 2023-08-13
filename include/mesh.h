#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <cglm/call.h>
#include "shader.h"
#include "texture.h"
#include <string.h>
#include "general.h"
#include <material.h>

void SetupMesh(Mesh *mesh)
{
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertex_len * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_len * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));

    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));

    glBindVertexArray(0);
}

Mesh InitMesh(Vertex *vertices, unsigned int *indices, unsigned int vertex_len, unsigned int index_len, unsigned int material_len, Material mat)
{
    Mesh mesh = {0};
    mesh.index_len = index_len;
    mesh.vertex_len = vertex_len;
    mesh.vertices = vertices;
    mesh.indices = indices;
    mesh.materials = mat;
    mesh.material_len = material_len;
    SetupMesh(&mesh);
    return mesh;
}

// bind textures and draw mesh
static inline void DrawMesh(Shader *shader, Mesh *mesh, const C_STRUCT aiScene *scene, Model *model)
{
    uint8_t diffuseNr = 1;
    uint8_t specularNr = 1;
    uint8_t normalNr = 1;
    uint8_t heightNr = 1;
    // printf("texlen: %u\n", mesh->texture_len);
    unsigned int s = 0;
    for (int types = 0; types < 2; types++)
    {
        for (size_t f = 0; f < model->loaded_textures_len - 1; f++)
        {
            glActiveTexture(GL_TEXTURE0 + types);
            int num_size = 1;
            char *number = malloc(10);
            // printf("type: %s\n", mesh->materials.maps[f].type);
            char *name = mesh->materials.maps[f].type;
            // printf("name: %s\n", name);
            //  name = malloc(strlen(mesh[i].materials.maps[i].type) + 1);
            if (strcmp(name, "texture_diffuse") == 0)
            {
                sprintf(number, "%u", diffuseNr++);
            }
            else if (strcmp(name, "texture_specular") == 0)
            {
                sprintf(number, "%u", specularNr++);
            }
            else if (strcmp(name, "texture_normal") == 0)
            {
                sprintf(number, "%u", normalNr++);
            }
            else if (strcmp(name, "texture_height") == 0)
            {
                sprintf(number, "%u", heightNr++);
            }
            int name_size = strlen(name);
            num_size = strlen(number);
            char *namer = malloc(name_size + num_size);
            number[num_size] = 0;
            memcpy(namer, name, name_size);
            memcpy(namer + name_size, number, num_size);
            // namer[name_size + num_size] = 0;
            // printf("namer: %s\n", namer);
            glUniform1i(glGetUniformLocation(shader->ID, "diffuse_maps"), s);
            glBindTexture(GL_TEXTURE_2D, mesh->materials.maps[f].ID);
            s++;
        }
        // glUniform1i(glGetUniformLocation(shader->ID, "material.diffuse"), s);
    }

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->index_len, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

#endif