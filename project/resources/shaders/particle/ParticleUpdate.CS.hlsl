static const uint PARTICLE_NONE = 0;
static const uint PARTICLE_DIFFUSION = 1;
static const uint PARTICLE_FOLLOW = 2;

struct Particle
{
    float32_t3 translate;
    float32_t3 scale;
    float32_t lifeTime;
    float32_t3 velocity;
    float32_t currentTime;
    float32_t4 color;
    uint32_t type;
    uint32_t id;
};

struct PerFrame
{
    float32_t time;
    float32_t deltaTime;
};

struct BulletData
{
    float32_t3 translate;
    uint32_t isAlive;
};

static const uint32_t kMaxParticles = 1024;
RWStructuredBuffer<Particle> gParticle : register(u0);
RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);
RWStructuredBuffer<uint32_t> gFreeList : register(u2);
ConstantBuffer<PerFrame> gParFrame : register(b1);
RWStructuredBuffer<BulletData> gBulletData : register(u3);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint32_t particleIndex = DTid.x;
    if(particleIndex < kMaxParticles)
    {
        if(gParticle[particleIndex].color.a == 0)
        {
            gParticle[particleIndex].scale = float32_t3(0.0f, 0.0f, 0.0f);
            int32_t freeListIndex;
            InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
            
            if((freeListIndex + 1) < kMaxParticles)
            {
                gFreeList[freeListIndex + 1] = particleIndex;
            }
            else
            {
                InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            }
        }
        else
        {
            float32_t alpha;
            
            switch (gParticle[particleIndex].type)
            {
                case PARTICLE_NONE:
                    gParticle[particleIndex].translate += gParticle[particleIndex].velocity * gParFrame.deltaTime;
                    gParticle[particleIndex].currentTime += gParFrame.deltaTime;
                    alpha = 1.0f - (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifeTime);
                    gParticle[particleIndex].color.a = saturate(alpha);
                
                    break;
                
                case PARTICLE_DIFFUSION:
                    gParticle[particleIndex].translate += gParticle[particleIndex].velocity * gParFrame.deltaTime;
                    gParticle[particleIndex].currentTime += gParFrame.deltaTime;
                    alpha = 1.0f - (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifeTime);
                    gParticle[particleIndex].color.a = saturate(alpha);
                
                    break;
                
                case PARTICLE_FOLLOW:
                
                
                if(gBulletData[gParticle[particleIndex].id].isAlive == 1)
                    {
                        gParticle[particleIndex].scale = float32_t3(0.0f, 0.0f, 0.0f);
                        int32_t freeListIndex;
                        InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
            
                        if ((freeListIndex + 1) < kMaxParticles)
                        {
                            gFreeList[freeListIndex + 1] = particleIndex;
                        }
                        else
                        {
                            InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
                        }
                    }
                else
                    {
                        gParticle[particleIndex].translate = gBulletData[gParticle[particleIndex].id].translate;
                        gParticle[particleIndex].currentTime += gParFrame.deltaTime;
                        alpha = 1.0f - (gParticle[particleIndex].currentTime / gParticle[particleIndex].lifeTime);
                        gParticle[particleIndex].color.a = saturate(alpha);
                    }
                
                    break;
                
            }
           
        }
    }
}