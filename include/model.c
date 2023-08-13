const C_STRUCT aiScene *scene = NULL;
GLuint *texture_ids = NULL;
Texture *textures;
unsigned int texture_max = 10;
unsigned int loaded_textures_max = 10;

static inline LoadMaterialTexturesFile(const C_STRUCT aiMaterial *mat, Model *model, Material *material)
{
    int total_tex_len = 0;
    C_ENUM aiTextureType type;
    char *type_name;
    for (int map_types = 0; map_types < 2; map_types++)
    {
        switch (map_types)
        {
        case 0:
            type = aiTextureType_DIFFUSE;
            type_name = "texture_diffuse";
            break;
        case 1:
            type = aiTextureType_SPECULAR;
            type_name = "texture_specular";
            break;
        }
        total_tex_len += aiGetMaterialTextureCount(mat, type);
    }
    material->maps = malloc(total_tex_len * sizeof(Texture));
    material->maps->type = NULL;
    for (int map_types = 0; map_types < 2; map_types++)
    {
        unsigned char *image_data = NULL;
        int width, height, components_per_pixel;
        switch (map_types)
        {
        case 0:
            type = aiTextureType_DIFFUSE;
            type_name = "texture_diffuse";
            break;
        case 1:
            type = aiTextureType_SPECULAR;
            type_name = "texture_specular";
            break;
        }
        unsigned int texture_lenger = 0;
        unsigned int texture_len = 0;
        texture_len = aiGetMaterialTextureCount(mat, type);
        // printf("texture_len %u\n", texture_len);
        C_STRUCT aiString str;
        for (size_t i = 0; i < texture_len; i++)
        {
            bool skip = false;
            aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);
            for (size_t j = 0; j < model->loaded_textures_len; j++)
            {
                if (strcmp(model->loaded_textures[j].path, str.data) == 0)
                {
                    material->maps[texture_lenger] = model->loaded_textures[j];
                    texture_lenger++;
                    skip = true;
                    printf("skip: %d\n", skip);
                    printf("NO DONT\n");
                    break;
                }
            }
            if (!skip)
            {
                printf("skip: %d\n", skip);
                printf("WTF\n");
                char *filename;
                filename = malloc(snprintf(NULL, 0, "%s/%s", model->directory, str.data));
                sprintf(filename, "%s/%s", model->directory, str.data);
                printf("directory: %s\n", model->directory);
                printf("mat string: %s\n", str.data);
                printf("filename: %s\n", filename);

                // printf("print bru: %s\n", str.data);
                Texture texture;
                if (strcmp(type_name, "texture_diffuse") == 0) // then gamma correct
                    texture = LoadTexture2D(filename, 16, state.gamma_correction);
                else
                    texture = LoadTexture2D(filename, 16, false);
                texture.type = type_name;
                texture.path = malloc(str.length);
                for (size_t q = 0; q < str.length; q++)
                    texture.path[q] = str.data[q];
                printf("print bru: %s\n", filename);
                if (model->loaded_textures_len >= loaded_textures_max - 1)
                {
                    loaded_textures_max += 10;
                    model->loaded_textures = realloc(model->loaded_textures, loaded_textures_max * sizeof(Texture));
                }
                // if (model->loaded_textures_len >= texture_max - 1)
                //{
                // texture_max += 10;
                // textures = realloc(textures, texture_max * sizeof(Texture));
                //}
                // textures[model->loaded_textures_len] = texture;
                material->maps[model->loaded_textures_len] = texture;
                model->loaded_textures[model->loaded_textures_len] = texture;
                model->loaded_textures_len++;
                model->texture_len++;
                printf("texture_type: %s\n", texture.type);
                printf("loaded_texture_len: %u\n", model->loaded_textures_len);
            }
        }
    }
}

static inline void LoadMaterialTextures(Mesh *mesh, unsigned int index)
{
    unsigned char *image_data = NULL;
    // mesh->textures = malloc(scene->mNumTextures * sizeof(GLuint));
    unsigned int *texture_ids = malloc(scene->mNumTextures * sizeof(unsigned int));
    glGenTextures(scene->mNumTextures, texture_ids);
    printf("texidsinit: %u\n", texture_ids[0]);
    int width, height, components_per_pixel;
    if (scene->mNumTextures != 0)
    {
        // please fix | embedded textures loading per object regardless of already loaded, skip already loaded textures
        // material index not texture index keep that in mind, i probably have to make a new material and index that, then somehow get it's texture and it's index
        for (int i = 0; i < scene->mNumTextures; i++)
        {
            if (scene->mTextures[i]->mHeight == 0)
                image_data = stbi_load_from_memory((unsigned char *)scene->mTextures[i]->pcData, scene->mTextures[i]->mWidth, &width, &height, &components_per_pixel, 0);
            else
                image_data = stbi_load_from_memory((unsigned char *)scene->mTextures[i]->pcData, scene->mTextures[i]->mWidth * scene->mTextures[i]->mHeight, &width, &height, &components_per_pixel, 0);

            GLenum format;
            if (components_per_pixel == 1)
                format = GL_RED;
            else if (components_per_pixel == 3)
                format = GL_RGB;
            else if (components_per_pixel == 4)
                format = GL_RGBA;
            if (image_data)
            {
                printf("bind");
                glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (scene->mTextures[i]->achFormatHint[0] & 0x01) ? GL_REPEAT : GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (scene->mTextures[i]->achFormatHint[0] & 0x01) ? GL_REPEAT : GL_CLAMP);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if (16 != 0)
                {
                    GLfloat value, max_anisotropy = 16;
                    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &value);
                    value = (value > max_anisotropy) ? max_anisotropy : value;
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, value);
                }
                stbi_image_free(image_data);
            }
            // mesh->textures[i].ID = texture_ids[i];
        }
    }
}

Mesh ProcessMesh(const C_STRUCT aiMesh *mesh, Model *model)
{
    unsigned int vertex_len = mesh->mNumVertices;
    unsigned int index_len = 0;
    const C_STRUCT aiFace *face = {0};

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        face = &mesh->mFaces[i];
        index_len += face->mNumIndices;
    }
    Vertex *vertices = malloc(vertex_len * sizeof(Vertex));
    unsigned int *indices = malloc(index_len * sizeof(unsigned int));
    unsigned int io = 0;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        face = &mesh->mFaces[i];
        for (unsigned int o = 0; o < face->mNumIndices; o++)
        {
            indices[io] = face->mIndices[o];
            io++;
        }
    }
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position[0] = mesh->mVertices[i].x;
        vertex.Position[1] = mesh->mVertices[i].y;
        vertex.Position[2] = mesh->mVertices[i].z;
        if (mesh->mNormals)
        {
            vertex.Normal[0] = mesh->mNormals[i].x;
            vertex.Normal[1] = mesh->mNormals[i].y;
            vertex.Normal[2] = mesh->mNormals[i].z;
        }
        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords[0] = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords[1] = mesh->mTextureCoords[0][i].y;

            vertex.Tangent[0] = mesh->mTangents[i].x;
            vertex.Tangent[1] = mesh->mTangents[i].y;
            vertex.Tangent[2] = mesh->mTangents[i].z;

            vertex.Bitangent[0] = mesh->mBitangents[i].x;
            vertex.Bitangent[1] = mesh->mBitangents[i].y;
            vertex.Bitangent[2] = mesh->mBitangents[i].z;
        }
        else
        {
            vertex.TexCoords[0] = 0.0f;
            vertex.TexCoords[1] = 0.0f;
        }
        vertices[i] = vertex;
    }

    Texture *diffuse_maps;
    Texture *specular_maps;
    Material mat;

    if (mesh->mMaterialIndex >= 0)
    {
        const C_STRUCT aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // Texture *diffuse_maps = malloc(1000 * texture_len * sizeof(Texture));
        LoadMaterialTexturesFile(material, model, &mat);
        // diffuse_maps = LoadMaterialTexturesFile(material, aiTextureType_DIFFUSE, "texture_diffuse", model);
        // specular_maps = LoadMaterialTexturesFile(material, aiTextureType_SPECULAR, "texture_specular", model);
        //  crash printf("index: %u\n", diffuse_maps[0].ID);
    }
    return InitMesh(vertices, indices, vertex_len, index_len, model->texture_len, mat);
}

void CheckModels(const C_STRUCT aiNode *node, unsigned int *meshes)
{
    *meshes += node->mNumMeshes;
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        CheckModels(node->mChildren[i], meshes);
    }
}

void ProcessNode(const C_STRUCT aiNode *node, Model *model)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        const C_STRUCT aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        // load mat
        // LoadMaterialTextures(&model->meshes[num_meshes], num_meshes);
        // model->texture_len = scene->mNumTextures;
        model->meshes[model->mesh_len] = ProcessMesh(mesh, model);
        model->mesh_len++;
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], model);
    }
}

static inline Model LoadModel(char *path)
{
    Model model = {0};
    model.mesh_max = 0;
    model.mesh_len = 0;
    model.loaded_textures_len = 0;
    scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    printf("path: %s\n", path);
    const C_STRUCT aiNode *node = scene->mRootNode;
    int size = strlen(path) - strlen(strrchr(path, '/'));
    model.directory = malloc(sizeof(char) * size + 1); // char is eh 1
    for (int i = 0; i < size; i++)
        model.directory[i] = path[i];
    model.directory[size] = 0;
    printf("dir: %s\n", model.directory);
    const char *extension = strrchr(path, '.');
    if (extension)
    {
        printf("opening file..\n");
    }
    if (!extension)
    {
        printf("Please provide a file with a valid extension.");
        return model;
    }

    if (AI_FALSE == aiIsExtensionSupported(extension))
    {
        printf("not supported file format");
        return model;
    }
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("Failed to load model. Please ensure that the specified file exists.");
        return model;
    }
    unsigned int meshes = 0;
    CheckModels(node, &meshes);
    model.mesh_max = meshes;
    printf("meshes %u\n", meshes);
    model.meshes = malloc(meshes * sizeof(Mesh));
    model.texture_len = 0;
    textures = malloc(texture_max * sizeof(Texture));
    model.loaded_textures = malloc(loaded_textures_max * sizeof(Texture));
    model.loaded_textures->path = NULL;
    ProcessNode(node, &model);
    return model;
}
static inline void DrawModel(Shader *shader, Model *model)
{
    for (unsigned int i = 0; i < model->mesh_max; i++)
        DrawMesh(shader, &model->meshes[i], scene, model);
}