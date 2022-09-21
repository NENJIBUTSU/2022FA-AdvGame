enum EntityType
{
    EntityType_Base,
    EntityType_Player,
    EntityType_Enemy,
    EntityType_Count,
};

struct BulletList
{
    int bulletListCount;
    int count;
    Bullet bullets[bulletListCount];
};

BulletList AddToList()
{
    int nextFreeIndex = count;
    if (bulletListCount > 0)
    {
        nextFreeIndex = bullets[bulletListCount - 1];
        bulletListCount--;
    }
    count++;
}

BulletList RemoveFromList(int indexToRemove)
{
    freeList[freeListCount] = indexToRemove;
    freeListCount++;
}

struct EntityInfo
{
    EntityType type;
    int32 generation;
    int32 indexInBuffer;
};

struct EntityHandle
{
    int32 id;
    int32 generation;

    EntityType type
};

struct Entity
{
    //All data that is shared by entities.
    vec2 position;
    vec2 lastPosition;

    bool markedForDeletion;
};

struct Player
{
    Entity entity;
    uint8 lives;
    vec4 color;
};

struct Enemy
{
    Entity entity;
    int8 direction;

};

struct Bullet
{
    Entity entity;
    int8 speed;
    real32 lifeTime;
};

struct EntityTypeBuffer
{
    int32 count;
    int32 entitySizeInBytes;
    int32 capacity;
    void* entities;
};

struct EntityManager
{
    int32 entityCapacity;
    int32 nextID;
    EntityInfo* entities;

    EntityTypeBuffer buffers[EntityType_Count];
};

void EntityManagerInit(EntityManager* em)
{
    em->entityCapacity = 1000;
    em->entities = (EntityInfo*)malloc(sizeof(EntityInfo) * em->entityCapacity);
    memset(em->entities, 0, sizeof(EntityInfo) * em->entityCapacity);

    em->nextID = 0;

    EntityTypeBuffer* baseBuffer = &em->buffers[EntityType_Base]; //BASE BUFFER
    baseBuffer->entitySizeInBytes = sizeof(Entity);
    baseBuffer->capacity = 100;
    baseBuffer->count = 0;
    baseBuffer->entities = malloc(baseBuffer->entitySizeInBytes * baseBuffer->capacity);

    EntityTypeBuffer* playerBuffer = &em->buffers[EntityType_Player]; //PLAYER BUFFER
    playerBuffer->entitySizeInBytes = sizeof(Player);
    playerBuffer->capacity = 4;
    playerBuffer->count = 0;
    playerBuffer->entities = malloc(playerBuffer->entitySizeInBytes * playerBuffer->capacity);

    EntityTypeBuffer* enemyBuffer = &em->buffers[EntityType_Enemy]; //ENEMY BUFFER
    enemyBuffer->entitySizeInBytes = sizeof(Enemy);
    enemyBuffer->capacity = 20;
    enemyBuffer->count = 0;
    enemyBuffer->entities = malloc(enemyBuffer->entitySizeInBytes * enemyBuffer->capacity);

}

EntityHandle AddEntity(EntityManager *em, EntityType type)
{
    EntityHandle h = {};

    EntityInfo* info = em->entities[em->nextID];
    info->type = type;

    EntityTypeBuffer* buffer = &em->buffers[type];
    info->indexInBuffer = buffer->count;

    buffer->count++;

    h.id = em->nextID;
    h.generation = info->generation;
    em->nextID++;

    return h;
};

EntityInfo* GetEntityInfo(EntityManager* em, EntityHandle h)
{
    if (h.id >= em->entityCapacity) //Check if entity capacity is at limit, if true return null info
    {
        return NULL;
    } 

    EntityInfo* info = &em->entities[h.id]; //Create EntityInfo pointer equal to the passed-in EntityHandle's ID.

    if (info->generation != h.generation) //If the info generation is not the same, return null
    {
        return NULL;
    }

    if (info->type != h.type) //If the type is not the same, return null
    {
        return NULL;
    }

    return info; //Otherwise return the EntityInfo
}

void* GetEntity(EntityManager *em, EntityHandle h)
{
    EntityInfo* info = GetEntityInfo(em, h);
    if (info == NULL)
    {
        return NULL;
    }

    EntityTypeBuffer* buffer = &em->buffers[info->type];

    return ((u8*)buffer->entities + (buffer->entitySizeInBytes * info->indexInBuffer));

}

void* DeleteEntity(EntityManager* em, EntityHandle h)
{
    EntityInfo* info = GetEntityInfo(em, h);
    if (info == NULL)
    {
        return NULL;
    }
    
    EntityTypeBuffer* buffer = &em->buffers[h.type];

    //get buffer count and subtract by 1
    //up the generation
    
    
    [info.id]
    info->indexInBuffer - 1;


    //Get index in buffer, increase generation?? and do pointer arithmetic on EntityInfo to set the count == index removed then count up?
    buffer->count++;

    return h;
}

struct GameData {
    EntityManager* entityManager;

    //Sprites
    Sprite playerSprite;
    Sprite enemySprite;
    Sprite bulletSprite;

    BulletList bulletList;
};

GameData *Data = NULL;


void LoadGameSprites()
{
    //This is hardcoded, probably better to rework it in the future through a SpriteData struct with a string path and Sprite name declaration.
    //Get a for loop of sprites in SpriteData array, iterate over and declare each.
    LoadSprite(&Data->playerSprite, "");
    LoadSprite(&Data->enemySprite, "");
    LoadSprite(&Data->bulletSprite, "");
}

void UpdatePositions()
{
    //Loop over enemies, make them move down the screen. If at edge of bounds then flip direction and move down 1.
    for (int i = 0; i < &Data->entityManager->enemyBuffer->count; i++)
    {
        Enemy* e = (Enemy*)GetEntity(&Data->entityManager->enemyBuffer, enemyHandle);
        e->entity.position = e->entity.lastPosition + V2(0, 0);
        e->entity.lastPosition = e->entity.position;
    }
}

void RenderGame()
{
    //Input rendering code here.
}


void MyInit() {
    //Initialize game data
    Game->myData = malloc(sizeof(GameData));
    memset(Game->myData, 0, sizeof(GameData));

    Data = (GameData *)Game->myData;

    //Load sprites
    LoadGameSprites();

    EntityManagerInit(&Data->entityManager);

    EntityHandle playerHandle = AddEntity(&Data->entityManager, EntityType_Player);
    Player* p = (Player*)GetEntity(&Data->entityManager, playerHandle);
    p->entity.position = V2();
    p->lives = 3;


}



void MyGameUpdate() {
    ClearColor(RGB(0.0f, 0.0f, 0.0f));

    Player* p = (Player*)GetEntity(&Data->entityManager, playerHandle);
    
}
