package com.xiaomi.parts;

import android.annotation.TargetApi;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Environment;
import android.service.quicksettings.TileService;
import android.widget.Toast;

import java.io.File;
import java.io.IOException;

import com.xiaomi.parts.R;

@TargetApi(24)
public class LogTile extends TileService {

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onTileAdded() {
        super.onTileAdded();
    }

    @Override
    public void onTileRemoved() {
        super.onTileRemoved();
    }

    @Override
    public void onStartListening() {
        super.onStartListening();
    }

    public Dialog logDialog() {
        CharSequence options[] = new CharSequence[]{
                "Logcat", "LogcatRadio", "Dmesg"};
        AlertDialog.Builder alertDialog = new AlertDialog.Builder(this);
        alertDialog.setTitle(R.string.quick_settings_log_tile_dialog_title);
        alertDialog.setItems(options, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                if (SuShell.detectValidSuInPath()) {
                    switch (which) {
                        case 0:
                            new CreateLogTask().execute(true, false, false);
                            break;
                        case 1:
                            new CreateLogTask().execute(false, true, false);
                            break;
                        case 2:
                            new CreateLogTask().execute(false, false, true);
                            break;
                    }
                } else {
                    Toast.makeText(LogTile.this,
                            R.string.cannot_get_su, Toast.LENGTH_SHORT).show();
                }

            }
        });
        return alertDialog.create();
    }

    public void makeLogcat() throws SuShell.SuDeniedException, IOException {
        final String LOGCAT_FILE = new File(Environment
            .getExternalStorageDirectory(), "LogCat.txt").getAbsolutePath();
        String command = "logcat -d";
        command += " > " + LOGCAT_FILE;
        SuShell.runWithSuCheck(command);
    }

    public void makeLogcatRadio() throws SuShell.SuDeniedException, IOException {
        final String LOGCAT_RADIO_FILE = new File(Environment
            .getExternalStorageDirectory(), "LogcatRadio.txt").getAbsolutePath();
        String command = "logcat -d -b radio";
        command += " > " + LOGCAT_RADIO_FILE;
        SuShell.runWithSuCheck(command);
    }

    public void makeDmesg() throws SuShell.SuDeniedException, IOException {
        final String DMESG_FILE = new File(Environment
            .getExternalStorageDirectory(), "Dmesg.txt").getAbsolutePath();
        String command = "dmesg -T";
        command += " > " + DMESG_FILE;
        SuShell.runWithSuCheck(command);
    }

    private class CreateLogTask extends AsyncTask<Boolean, Void, Void> {

        private Exception mException = null;

        @Override
        protected Void doInBackground(Boolean... params) {
            try {
                if (params[0]) {
                    makeLogcat();
                }
                if (params[1]) {
                    makeLogcatRadio();
                }
                if (params[2]) {
                    makeDmesg();
                }
            } catch (SuShell.SuDeniedException e) {
                mException = e;
            } catch (IOException e) {
                e.printStackTrace();
                mException = e;
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            if (mException instanceof SuShell.SuDeniedException) {
                Toast.makeText(LogTile.this, R.string.cannot_get_su,
                        Toast.LENGTH_LONG).show();
                return;
            }
        }
    }

    @Override
    public void onClick() {
        super.onClick();
        showDialog(logDialog());
    }
}
