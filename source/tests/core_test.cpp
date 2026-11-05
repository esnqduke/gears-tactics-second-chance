#include "../mod_core.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#c); std::exit(1); } } while(0)
template<class T, size_t N> void put(unsigned char (&b)[N], size_t off, T value) { CHECK(off+sizeof value<=N); std::memcpy(b+off,&value,sizeof value); }
template<size_t N> uintptr_t ptr(unsigned char (&b)[N]) { return reinterpret_cast<uintptr_t>(b); }
int main() {
    auto image=static_cast<unsigned char*>(VirtualAlloc(nullptr,0xb57aa00,MEM_RESERVE,PAGE_READWRITE)); CHECK(image);
    for (size_t rva : {size_t(0x95ae000),size_t(0x9174000),size_t(0x917e000),size_t(0x9145000),size_t(0x90fc000),size_t(0x9100000)}) CHECK(VirtualAlloc(image+rva,4096,MEM_COMMIT,PAGE_READWRITE));
    unsigned char engine[0xa00]{},meta[0x900]{},progression[0x500]{},context[0x400]{},world[0x200]{},mission[0x600]{};
    unsigned char squad[0x200]{},roster[0x300]{},soldier[0x330]{},enemy[0x330]{},hero[0x330]{},heroTemplate[0x330]{};
    unsigned char instanceClass[0x80]{},missionClass[0x80]{},squadClass[0x80]{},rosterClass[0x80]{},soldierClass[0x80]{},enemyClass[0x80]{};
    *reinterpret_cast<uintptr_t*>(image+0x95ae410)=ptr(engine);
    *reinterpret_cast<uintptr_t*>(image+0x91746a0)=ptr(missionClass);
    *reinterpret_cast<uintptr_t*>(image+0x9100420)=ptr(instanceClass);
    put(engine,0x10,ptr(instanceClass));
    *reinterpret_cast<uintptr_t*>(image+0x9145fa0)=ptr(squadClass);
    *reinterpret_cast<uintptr_t*>(image+0x90fcc00)=ptr(rosterClass);
    *reinterpret_cast<uintptr_t*>(image+0x917e430)=ptr(soldierClass);
    put(engine,0x988,ptr(meta)); put(engine,0x30,ptr(context)); put(context,0x3e0,ptr(world));
    put(world,0x108,ptr(mission)); put(mission,0x10,ptr(missionClass)); mission[0x5f0]=3;
    put(meta,0x7e8,ptr(progression)); progression[0x430]=3;
    put(meta,0x7d8,ptr(squad)); put(meta,0x7d0,ptr(roster));
    put(squad,0x10,ptr(squadClass)); put(roster,0x10,ptr(rosterClass));
    put(hero,0x10,ptr(soldierClass)); put(heroTemplate,0x10,ptr(soldierClass));
    put(hero,0x31c,1); put(heroTemplate,0x31c,-1);
    uintptr_t heroes[]{ptr(hero)},templates[]{ptr(heroTemplate)};
    put(roster,0x280,reinterpret_cast<uintptr_t>(heroes));put(roster,0x288,1);put(roster,0x28c,1);
    put(roster,0x270,reinterpret_cast<uintptr_t>(templates));put(roster,0x278,1);put(roster,0x27c,1);
    put(soldier,0x10,ptr(soldierClass)); put(enemy,0x10,ptr(enemyClass));
    int defaults[]{4,4,3,1,4}; std::memcpy(squad+0x180,defaults,sizeof defaults);
    uintptr_t members[]{ptr(soldier),ptr(enemy)};
    put(roster,0x210,reinterpret_cast<uintptr_t>(members)); put(roster,0x218,2); put(roster,0x21c,2);
    put(soldier,0x31c,1); put(enemy,0x31c,1);
    unsigned char originalSoldier[sizeof soldier]; std::memcpy(originalSoldier,soldier,sizeof soldier);
    DbnoRuntime r; r.base=reinterpret_cast<uintptr_t>(image);
    put(world,0x108,uintptr_t(0)); CHECK(r.tick()==0); put(world,0x108,ptr(mission));
    for(unsigned char d=0;d<3;++d){progression[0x430]=d;CHECK(r.tick()==0);} progression[0x430]=3;
    mission[0x5f0]=2; CHECK(r.tick()==0); mission[0x5f0]=3;
    CHECK(std::memcmp(soldier,originalSoldier,sizeof soldier)==0);
    CHECK(r.tick()==3); CHECK(r.tick()==0);
    int value=0; CHECK(DbnoRuntime::read(ptr(hero)+0x31c,value)&&value==2); CHECK(DbnoRuntime::read(ptr(heroTemplate)+0x31c,value)&&value==-1); CHECK(DbnoRuntime::read(ptr(soldier)+0x31c,value)&&value==2);
    CHECK(DbnoRuntime::read(ptr(enemy)+0x31c,value)&&value==1);
    put(originalSoldier,0x31c,2); CHECK(std::memcmp(soldier,originalSoldier,sizeof soldier)==0);
    defaults[3]=2; CHECK(std::memcmp(squad+0x180,defaults,sizeof defaults)==0);
    put(soldier,0x31c,3); CHECK(r.tick()==1);
    for(int special : {-1,0}) {put(soldier,0x31c,special);CHECK(r.tick()==0);CHECK(DbnoRuntime::read(ptr(soldier)+0x31c,value)&&value==special);}
    put(soldier,0x31c,1);
    for(unsigned flags : {0x10u,0x8000u,0x10000u}) {put(soldier,8,flags); CHECK(r.tick()==0);}
    put(soldier,8,0u);
    put(roster,0x218,3000); CHECK(r.tick()==0); put(roster,0x218,2);
    put(roster,0x210,uintptr_t(0xdead0000));CHECK(r.tick()==0);put(roster,0x210,reinterpret_cast<uintptr_t>(members));
    CHECK(r.tick()==1);
    CHECK(!DbnoRuntime::read(uintptr_t(0xdead0000),value));
    VirtualFree(image,0,MEM_RELEASE);
    std::puts("PASS: data-only runtime; startup/difficulty/class guards, one-DBNO limit, special values, reload, invalid pointers, unchanged adjacent data.");
}
