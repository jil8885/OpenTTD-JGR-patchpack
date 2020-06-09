/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file signs.cpp Handling of signs. */

#include "stdafx.h"
#include "landscape.h"
#include "signs_base.h"
#include "signs_func.h"
#include "strings_func.h"
#include "core/pool_func.hpp"
#include "viewport_kdtree.h"

#include "table/strings.h"

#include "safeguards.h"

/** Initialize the sign-pool */
SignPool _sign_pool("Sign");
INSTANTIATE_POOL_METHODS(Sign)

/**
 * Creates a new sign
 */
Sign::Sign(Owner owner)
{
	this->owner = owner;
}

/** Destroy the sign */
Sign::~Sign()
{
	if (CleaningPool()) return;

	DeleteRenameSignWindow(this->index);
}

/**
 * Update the coordinate of one sign
 */
void Sign::UpdateVirtCoord()
{
	Point pt = RemapCoords(this->x, this->y, this->z);

	if (_viewport_sign_kdtree_valid && this->sign.kdtree_valid) _viewport_sign_kdtree.Remove(ViewportSignKdtreeItem::MakeSign(this->index));

	SetDParam(0, this->index);
	bool shown = HasBit(_display_opt, DO_SHOW_SIGNS) && !(this->IsCompetitorOwned() && !HasBit(_display_opt, DO_SHOW_COMPETITOR_SIGNS));
	this->sign.UpdatePosition(shown ? ZOOM_LVL_DRAW_SPR : ZOOM_LVL_END, pt.x, pt.y - 6 * ZOOM_LVL_BASE, STR_WHITE_SIGN);

	if (_viewport_sign_kdtree_valid) _viewport_sign_kdtree.Insert(ViewportSignKdtreeItem::MakeSign(this->index));
}

/** Update the coordinates of all signs */
void UpdateAllSignVirtCoords()
{
	for (Sign *si : Sign::Iterate()) {
		si->UpdateVirtCoord();
	}
}
