#pragma once
#include <windows.h>
#include <cstdint>

struct DbnoRuntime {
    uintptr_t base=0;
    static constexpr int TargetSteps=2;
    template<class T> static bool read(uintptr_t address,T& result) {
        SIZE_T n=0;
        return address>=65536 && ReadProcessMemory(GetCurrentProcess(),reinterpret_cast<const void*>(address),&result,sizeof result,&n) && n==sizeof result;
    }
    struct Identity { unsigned flags; int index; uintptr_t klass; };
    struct Frame { uintptr_t instance=0,meta=0,world=0,mission=0,roster=0,squad=0; };
    bool isA(uintptr_t object,uintptr_t classRva,Identity* identity=nullptr) const {
        uintptr_t expected=0;
        Identity id{};
        // RF_ClassDefaultObject, RF_BeginDestroyed, RF_FinishDestroyed.
        if(!object || !read(base+classRva,expected) || !expected || !read(object+8,id) || (id.flags&0x18010)) return false;
        uintptr_t current=id.klass;
        for(int i=0;current && i<32;++i) {
            if(current==expected) { if(identity)*identity=id; return true; }
            if(!read(current+0x30,current)) return false;
        }
        return false;
    }
    bool frame(Frame& f) const {
        uintptr_t context=0,progression=0;
        unsigned char difficulty=255,missionDifficulty=255;
        return base && read(base+0x95ae410,f.instance) && f.instance &&
            isA(f.instance,0x9100420) && read(f.instance+0x30,context) && context && read(context+0x3e0,f.world) && f.world &&
            read(f.world+0x108,f.mission) && isA(f.mission,0x91746a0) &&
            read(f.mission+0x5f0,missionDifficulty) && missionDifficulty==3 &&
            read(f.instance+0x988,f.meta) && f.meta &&
            read(f.meta+0x7e8,progression) && progression &&
            read(progression+0x430,difficulty) && difficulty==3 &&
            read(f.meta+0x7d0,f.roster) && isA(f.roster,0x90fcc00) &&
            read(f.meta+0x7d8,f.squad) && isA(f.squad,0x9145fa0);
    }
    bool sameFrame(const Frame& before) const {
        Frame now{};
        return frame(now) && before.instance==now.instance && before.meta==now.meta &&
            before.world==now.world && before.mission==now.mission && before.roster==now.roster && before.squad==now.squad;
    }
    bool update(const Frame& f,uintptr_t object,uintptr_t offset,uintptr_t classRva) const {
        int value=0; Identity before{},after{};
        if(!isA(object,classRva,&before) || !read(object+offset,value) || value<=0 || value>4 || value==TargetSteps ||
            !sameFrame(f) || !isA(object,classRva,&after) || before.index!=after.index || before.klass!=after.klass || before.flags!=after.flags) return false;
        int current=0;
        if(!read(object+offset,current) || current!=value) return false;
        SIZE_T n=0; const int replacement=TargetSteps;
        return WriteProcessMemory(GetCurrentProcess(),reinterpret_cast<void*>(object+offset),&replacement,sizeof replacement,&n) && n==sizeof replacement;
    }
    unsigned tick() const {
        Frame f{};
        if(!frame(f)) return 0;
        int defaults[5]{};
        if(!read(f.squad+0x180,defaults) || defaults[0]!=4 || defaults[1]!=4 || defaults[2]!=3 || defaults[4]!=4) return 0;
        unsigned changed=update(f,f.squad,0x18c,0x9145fa0)?1u:0u;
        // SoldierRoster, SpecialHeroCharacters, AvailableSpecialHeroCharacters.
        const uintptr_t lists[]{0x210,0x270,0x280};
        for(const auto offset:lists) {
            struct Array {uintptr_t data;int count,capacity;} list{};
            if(!read(f.roster+offset,list) || list.count<0 || list.count>2048 || list.capacity<list.count || list.capacity>8192)continue;
            uintptr_t characters[2048]{};
            SIZE_T n=0;
            const SIZE_T bytes=static_cast<SIZE_T>(list.count)*sizeof(uintptr_t);
            if(bytes && (!ReadProcessMemory(GetCurrentProcess(),reinterpret_cast<void*>(list.data),characters,bytes,&n) || n!=bytes))continue;
            for(int i=0;i<list.count;++i) {
                Array current{};
                if(!read(f.roster+offset,current) || current.data!=list.data || current.count!=list.count)break;
                uintptr_t member=0;
                if(read(list.data+static_cast<uintptr_t>(i)*8,member) && member==characters[i] &&
                    update(f,member,0x31c,0x917e430))++changed;
            }
        }
        return changed;
    }
};
