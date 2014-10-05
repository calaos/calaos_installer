#!/bin/bash

QTVERSION="5.3"
QTPATH="/home/$USER/Qt/$QTVERSION/gcc_64"

VERSION=`../calaos_installer --version | tail -1 | tr -d '\t' `

echo "Creating package..."
mkdir -p calaos_installer_$VERSION/.lib

cp ../calaos_installer calaos_installer_$VERSION/.calaos_installer.bin

for l in libQt5DBus.so.5 libQt5Gui.so.5 libQt5Network.so.5 libQt5Core.so.5 libQt5Svg.so.5 libQt5Xml.so.5 libQt5PrintSupport.so.5 libQt5Widgets.so.5 libicudata.so.52  libicui18n.so.52 libicuuc.so.52
do
 	cp $QTPATH/lib/$l calaos_installer_$VERSION/.lib/
done

for l in /usr/lib/libstdc++.so.6 /usr/lib/libssl.so.1.0.0 /usr/lib/libcrypto.so.1.0.0
do
	cp $l calaos_installer_$VERSION/.lib/
done

cp -R $QTPATH/plugins/iconengines calaos_installer_$VERSION
cp -R $QTPATH/plugins/imageformats calaos_installer_$VERSION
cp -R $QTPATH/plugins/platforms calaos_installer_$VERSION

strip calaos_installer_$VERSION/.calaos_installer.bin calaos_installer_$VERSION/.lib/* calaos_installer_$VERSION/iconengines/* calaos_installer_$VERSION/imageformats/* calaos_installer_$VERSION/platforms/*

cat > calaos_installer_$VERSION/calaos_installer << "EOF"
#!/bin/sh
link=`readlink "$0"`

if [ "${link}x" = "x"  ]; then
    dirname=`dirname "$0"`   
else
    dirname=`dirname "$0"`/`dirname "$link"`
fi

app=`basename "$0"`

tmp="${dirname#?}"
if [ "${dirname%$tmp}" != "/" ]; then
    dirname="$PWD/$dirname"
fi
LD_LIBRARY_PATH="$dirname/.lib"
QT_PLUGIN_PATH="$dirname"
export LD_LIBRARY_PATH
export QT_PLUGIN_PATH
"$dirname/.$app.bin" "$@"
EOF

chmod +x calaos_installer_$VERSION/calaos_installer

rm -f calaos-installer_linux_$VERSION.tar.bz2
tar cjf calaos-installer_linux_$VERSION.tar.bz2 calaos_installer_$VERSION

echo "Done."
echo "Package: calaos-installer_linux_$VERSION.tar.bz2"

