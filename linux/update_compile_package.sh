#!/bin/bash

ARCH=32
[ "$1" = "x64" ] && ARCH=64

type=experimental
[ "$2" = "TESTING" ] && type=testing
[ "$2" = "STABLE" ] && type=stable

if [ $ARCH = 32 ]
then
    QTPATH="/opt/Qt5.4.0/5.4/gcc"
else
    QTPATH="/opt/Qt5.4.0/5.4/gcc_64"
fi

(
cd ..
mkdir build
cd build
$QTPATH/bin/qmake ../calaos_installer.pro
make -j2
);

VERSION=`../build/calaos_installer --version | tail -1 | tr -d '\t' `

echo "Creating package..."
mkdir -p calaos_installer_$VERSION/.lib

cp ../build/calaos_installer calaos_installer_$VERSION/.calaos_installer.bin

for l in libQt5DBus.so.5 \
         libQt5Gui.so.5 \
         libQt5Network.so.5 \
         libQt5Core.so.5 \
         libQt5Svg.so.5 \
         libQt5Xml.so.5 \
         libQt5PrintSupport.so.5 \
         libQt5Widgets.so.5 \
         libicudata.so.53 \
         libicui18n.so.53 \
         libicuuc.so.53
do
 	cp $QTPATH/lib/$l calaos_installer_$VERSION/.lib/
done

for l in /usr/lib/i386-linux-gnu/libstdc++.so.6 \
         /usr/lib/i386-linux-gnu/i586/libssl.so.1.0.0 \
         /usr/lib/i386-linux-gnu/i586/libcrypto.so.1.0.0
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

rm -f calaos_installer_linux_$VERSION.tar.xz
tar cJfv calaos_installer_linux_$VERSION.tar.xz calaos_installer_$VERSION

echo "Done."
echo "Package: calaos_installer_linux_$VERSION.tar.xz"

scp calaos_installer_linux_$VERSION.tar.xz uploader@calaos.fr:/home/raoul/www/download.calaos.fr/$type/calaos_installer/

