package org.lineageos.settings.dirac;

import android.service.quicksettings.Tile;
import android.service.quicksettings.TileService;

public class DiracTileService extends TileService {

    @Override
    public void onStartListening() {

        Tile tile = getQsTile();
        if (DiracUtils.isDiracEnabled()) {
            tile.setState(Tile.STATE_ACTIVE);
        } else {
            tile.setState(Tile.STATE_INACTIVE);
        }

        tile.updateTile();

        super.onStartListening();
    }

    @Override
    public void onClick() {
        Tile tile = getQsTile();
        if (DiracUtils.isDiracEnabled()) {
            DiracUtils.setEnabled(false);
            tile.setState(Tile.STATE_INACTIVE);
        } else {
            DiracUtils.setEnabled(true);
            tile.setState(Tile.STATE_ACTIVE);
        }
        tile.updateTile();
        super.onClick();
    }
}
