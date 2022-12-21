package org.waltercoincore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class WaltercoinQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File waltercoinDir = new File(getFilesDir().getAbsolutePath() + "/.waltercoin");
        if (!waltercoinDir.exists()) {
            waltercoinDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
