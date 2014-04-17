# How to add new language

First edit the project file calaos_installer.pro and add the wanted language to the TRANSLATIONS var:

TRANSLATIONS = \
    ....
    lang/calaos_installer_XX.ts

XX is the language code you want to add.

Then you need to create the new .ts file. Do this by running in QtCreator:
Tools -> External -> Linguist -> Update translations

Or manually by running:
```
lupdate calaos_installer.pro
```

The second step is to build the .qm (compiled translation file) and add it to the ressource file.
From QtCreator:
Tools -> External -> Linguist -> Release translations

Or manually by running:
```
lrelease calaos_installer.pro
```

You can then add the new calaos_installer_XX.qm file to lang/lang_ressources.qrc

## Important note
Do not add the .qm file in git, the file is compiled automatically at build time.
