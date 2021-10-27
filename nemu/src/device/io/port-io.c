#include <device/map.h>

#define PORT_IO_SPACE_MAX 65535

#define NR_MAP 16
static IOMap maps[NR_MAP] = {};
static int nr_map = 0;

/* device interface */
void add_pio_map(const char *name, ioaddr_t addr, void *space, uint32_t len, io_callback_t callback) {
	assert(nr_map < NR_MAP);
	assert(addr + len <= PORT_IO_SPACE_MAX);
	maps[nr_map] = (IOMap){ .name = name, .low = addr, .high = addr + len - 1,
		.space = space, .callback = callback };
	Log("Add port-io map '%s' at [" FMT_PADDR ", " FMT_PADDR "]",
			maps[nr_map].name, maps[nr_map].low, maps[nr_map].high);

	nr_map ++;
}


bool dtrace_enable(IOMap*);
/* CPU interface */
uint32_t pio_read(ioaddr_t addr, int len) {
	assert(addr + len - 1 < PORT_IO_SPACE_MAX);
	int mapid = find_mapid_by_addr(maps, nr_map, addr);
	assert(mapid != -1);
	word_t ret = map_read(addr, len, &maps[mapid]);
	#ifdef CONFIG_DTRACE
	static int count = 1;
	if (dtrace_enable(&maps[mapid]))
	printf("[read |pio] count:%-6d name:%s data:0x%-9x %-16d\n", count++, maps[mapid].name, ret, ret);
	#endif
	return ret;
}

void pio_write(ioaddr_t addr, int len, uint32_t data) {
	assert(addr + len - 1 < PORT_IO_SPACE_MAX);
	int mapid = find_mapid_by_addr(maps, nr_map, addr);
	assert(mapid != -1);
	#ifdef CONFIG_DTRACE
	static int count = 1;
	if (dtrace_enable(&maps[mapid]))
	printf("[write|pio] count:%-6d name:%s data:0x%-9x %-16d\n", count++, maps[mapid].name, data, data);
	#endif
	map_write(addr, len, data, &maps[mapid]);
}
