/***************************************************************************

Atari Drag Race video emulation

***************************************************************************/

#include "emu.h"
#include "includes/dragrace.h"


static TILE_GET_INFO( get_tile_info )
{
	dragrace_state *state = machine.driver_data<dragrace_state>();
	UINT8 code = state->m_playfield_ram[tile_index];
	int num = 0;
	int col = 0;

	num = code & 0x1f;

	if ((code & 0xc0) == 0x40)
		num |= 0x20;

	switch (code & 0xA0)
	{
	case 0x00:
		col = 0;
		break;
	case 0x20:
		col = 1;
		break;
	case 0x80:
		col = (code & 0x40) ? 1 : 0;
		break;
	case 0xA0:
		col = (code & 0x40) ? 3 : 2;
		break;
	}

	SET_TILE_INFO(((code & 0xA0) == 0x80) ? 1 : 0, num, col, 0);
}


VIDEO_START( dragrace )
{
	dragrace_state *state = machine.driver_data<dragrace_state>();
	state->m_bg_tilemap = tilemap_create(machine, get_tile_info, tilemap_scan_rows, 16, 16, 16, 16);
}


SCREEN_UPDATE( dragrace )
{
	dragrace_state *state = screen->machine().driver_data<dragrace_state>();
	int y;

	tilemap_mark_all_tiles_dirty(state->m_bg_tilemap);

	for (y = 0; y < 256; y += 4)
	{
		rectangle rect = *cliprect;

		int xl = state->m_position_ram[y + 0] & 15;
		int xh = state->m_position_ram[y + 1] & 15;
		int yl = state->m_position_ram[y + 2] & 15;
		int yh = state->m_position_ram[y + 3] & 15;

		tilemap_set_scrollx(state->m_bg_tilemap, 0, 16 * xh + xl - 8);
		tilemap_set_scrolly(state->m_bg_tilemap, 0, 16 * yh + yl);

		if (rect.min_y < y + 0) rect.min_y = y + 0;
		if (rect.max_y > y + 3) rect.max_y = y + 3;

		tilemap_draw(bitmap, &rect, state->m_bg_tilemap, 0, 0);
	}
	return 0;
}
