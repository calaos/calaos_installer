#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <version>"
	exit 1
fi

VERSION=$1

#update
echo "Updating from svn..."
(cd calaos_installer; svn update;)

echo "Compile..."
(cd calaos_installer; make -j4;)

echo "Creating package..."
mkdir -p calaos_installer_$VERSION/.lib

cp calaos_installer/calaos_installer calaos_installer_$VERSION/.calaos_installer.bin

for l in libQtDBus.so.4 libQtGui.so.4 libQtNetwork.so.4 libQtCore.so.4 libQtSvg.so.4 libQtXml.so.4
do
 	cp /usr/local/Trolltech/Qt-4.7.1/lib/$l calaos_installer_$VERSION/.lib/
done

for l in /usr/lib/libssl.so.0.9.8 /usr/lib/libexpat.so.1 /usr/lib/libpng12.so.0 /usr/lib/libjpeg.so.62
do
	cp $l calaos_installer_$VERSION/.lib/
done

cp -R /usr/local/Trolltech/Qt-4.7.1/plugins/styles calaos_installer_$VERSION
cp -R /usr/local/Trolltech/Qt-4.7.1/plugins/iconengines calaos_installer_$VERSION
cp -R /usr/local/Trolltech/Qt-4.7.1/plugins/imageformats calaos_installer_$VERSION

strip calaos_installer_$VERSION/.calaos_installer.bin calaos_installer_$VERSION/.lib/* calaos_installer_$VERSION/styles/* calaos_installer_$VERSION/iconengines/* calaos_installer_$VERSION/imageformats/*

cat > calaos_installer_$VERSION/calaos_installer << "EOF"
#!/bin/sh
link=`readlink $0`

if [ "${link}x" = "x"  ]; then
    dirname=`dirname $0`   
else
    dirname=`dirname $0`/`dirname $link`
fi

app=`basename $0`

tmp="${dirname#?}"
if [ "${dirname%$tmp}" != "/" ]; then
    dirname=$PWD/$dirname
fi
LD_LIBRARY_PATH=$dirname/.lib
QT_PLUGIN_PATH=$dirname
export LD_LIBRARY_PATH
export QT_PLUGIN_PATH
$dirname/.$app.bin "$@"
EOF

chmod +x calaos_installer_$VERSION/calaos_installer

rm -f CalaosInstaller_linux_$VERSION.tar.bz2
tar cjf CalaosInstaller_linux_$VERSION.tar.bz2 calaos_installer_$VERSION

echo "Done."
echo "Package: CalaosInstaller_linux_$VERSION.tar.bz2"

