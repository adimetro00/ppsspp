#include "Common/ChunkFile.h"
#include "Core/FileSystems/MetaFileSystem.h"
#include "Core/HW/MemoryStick.h"
#include "Core/System.h"

// MS and FatMS states.
static MemStickState memStickState;
static MemStickFatState memStickFatState;
static u64 memStickSize;

void MemoryStick_DoState(PointerWrap &p)
{
	auto s = p.Section("MemoryStick", 1, 2);
	if (!s)
		return;

	p.Do(memStickState);
	p.Do(memStickFatState);
	if (s >= 2)
		p.Do(memStickSize);
	else
		memStickSize = 1ULL * 1024 * 1024 * 1024;

}

MemStickState MemoryStick_State()
{
	return memStickState;
}

MemStickFatState MemoryStick_FatState()
{
	return memStickFatState;
}

u64 MemoryStick_SectorSize()
{
	return 32 * 1024; // 32KB
}

u64 MemoryStick_FreeSpace()
{
	u64 freeSpace = pspFileSystem.FreeSpace("ms0:/");
	if (freeSpace < memStickSize)
		return freeSpace;
	return memStickSize;
}

void MemoryStick_SetFatState(MemStickFatState state)
{
	memStickFatState = state;
}

void MemoryStick_Init()
{
	memStickState = PSP_MEMORYSTICK_STATE_DRIVER_READY;
	memStickFatState = PSP_FAT_MEMORYSTICK_STATE_ASSIGNED;
	memStickSize = 8ULL * 1024 * 1024 * 1024; // 8GB
}
