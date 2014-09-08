#!/bin/bash

QTPATH="/home/$USER/Qt/5.3/gcc_64"
QTVERSION="5.3.1"
VERSION=`../calaos_installer --version | tail -1 | tr -d '\t' `

echo "Creating package..."
mkdir -p calaos_installer_$VERSION/.lib

cp ../calaos_installer calaos_installer_$VERSION/.calaos_installer.bin

for l in libQt5DBus.so libQt5Gui.so libQt5Network.so libQt5Core.so libQt5Svg.so libQt5Xml.so
do
 	cp $QTPATH/lib/$l.$QTVERSION calaos_installer_$VERSION/.lib/
done

#for l in /usr/lib/libssl.so.0.9.8 /usr/lib/libexpat.so.1 /usr/lib/libpng12.so.0 /usr/lib/libjpeg.so.62
#do
#	cp $l calaos_installer_$VERSION/.lib/
#done

cp -R $QTPATH/plugins/iconengines calaos_installer_$VERSION
cp -R $QTPATH/plugins/imageformats calaos_installer_$VERSION

strip calaos_installer_$VERSION/.calaos_installer.bin calaos_installer_$VERSION/.lib/* calaos_installer_$VERSION/iconengines/* calaos_installer_$VERSION/imageformats/*

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

