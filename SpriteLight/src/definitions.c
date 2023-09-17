int GetRandomValue(int min, int max)
{
    if (min > max)
    {
        int tmp = max;
        max = min;
        min = tmp;
    }
    if ((unsigned int)(max - min) > (unsigned int)RAND_MAX)
    {
        printf("value exceeds %i", RAND_MAX);
    }
    return (rand() % (abs(max - min) + 1) + min);
}

float Lerp(float start, float end, float amount)
{
    float result = start + amount * (end - start);

    return result;
}

Rectangle *CreateRectangle(int x, int y, int width, int height, Texture tex)
{
    Rectangle *rec = malloc(sizeof(Rectangle));
    *rec = (Rectangle){x, y, width, height, tex.ID};
    if (state.recs_len + 1 > state.recs_max)
    {
        state.recs_max += 20;
        state.recs = realloc(state.recs, state.recs_max);
    }
    state.recs[state.recs_len] = rec;
    state.recs_len++;
    return rec;
}