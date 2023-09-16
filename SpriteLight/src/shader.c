char *ReadTextFile(char *path)
{
    void *file;
    long fsize;
    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);
    path = malloc(fsize + 1);
    fread(path, 1, fsize, file);
    path[fsize] = 0;
    printf("reading");
    return path;
}

char* FormatShaderUniform(const char* uniform_name, int index)
{
    int buffer_size = snprintf(NULL, 0, "%s[%d].%s", uniform_name, index, uniform_name) + 1;
    char* buffer = malloc(buffer_size);
    snprintf(buffer, buffer_size, "%s[%d].%s", uniform_name, index, uniform_name);
    return buffer;
}

Shader LoadShader(char *vertex_path, char *fragment_path)
{
    Shader shader = {0};
    char* vertex_shader_source = ReadTextFile(vertex_path);
    char* fragment_shader_source = ReadTextFile(fragment_path);

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", infoLog);
    };
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", infoLog);
    };
    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, vertex);
    glAttachShader(shader.ID, fragment);
    glLinkProgram(shader.ID);
    glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s", infoLog);
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return shader;
}
void UseShader(Shader shader)
{
    glUseProgram(shader.ID);
}
void SetShaderBool(int Shader_ID, const char *name, bool value)
{
    glUniform1i(glGetUniformLocation(Shader_ID, name), (int)value);
}
void SetShaderInt(int Shader_ID, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(Shader_ID, name), value);
}
void SetShaderFloat(int Shader_ID, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(Shader_ID, name), value);
}
void SetShaderMat4(int Shader_ID, const char *name, mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(Shader_ID, name), 1, GL_FALSE, matrix);
}
void SetShaderVec2(int Shader_ID, const char *name, vec2 vector)
{
    glUniform2f(glGetUniformLocation(Shader_ID, name), vector[0], vector[1]);
}
void SetShaderVec3(int Shader_ID, const char *name, vec3 vector)
{
    glUniform3f(glGetUniformLocation(Shader_ID, name), vector[0], vector[1], vector[2]);
}

inline void SetShaderVec4(int Shader_ID, const char *name, vec4 value)
{
    glUniform4f(glGetUniformLocation(Shader_ID, name), value[0], value[1], value[2], value[3]);
}

void SetShaderVec3v(int Shader_ID, const char *name, vec3 *vector, int amount)
{
    glUniform3fv(glGetUniformLocation(Shader_ID, name), amount, vector);
}